#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include <unistd.h> // header for sysconf

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;

  float memTotal   = 0.f;
  float memFree    = 0.f;
  float memBuffers = 0.f;
  float memCached  = 0.f;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memTotal = std::stof(value);
        }
        if (key == "MemFree:") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memFree = std::stof(value);
        }
        if (key == "Buffers:") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memBuffers = std::stof(value);
        }
        if (key == "Cached:") {
          std::replace(value.begin(), value.end(), '_', ' ');
          memCached = std::stof(value);
        }
      }
    }
  }
  return ((memTotal - memBuffers - memCached) - memFree)/(memTotal - memBuffers - memCached);
}

/**
 * Function returns the uptime of the system.
 * @param[out] uptime of the system
*/
long LinuxParser::UpTime() { 
  string line;
  long   uptime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime;
      return uptime; 
    }
  }
  return uptime;
}

long LinuxParser::ActiveJiffies(int pid) {
  vector<string> values;  // [14=utime, 15=stime= 16=cutime, 17=cstime]
  long activeJiffies = 0;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string tempVal;
    int index = 0;
    while(linestream >> tempVal && index < 18){
      values.push_back(tempVal);
      index++;
    }
  }
  try {
    activeJiffies = stol(values[13]) + stol(values[14]) + stol(values[15]) + stol(values[16]);
  }
  catch (...) {
    activeJiffies = 0;
  }
  return activeJiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> times;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if ( key == "cpu" ) {
        while (linestream >> value) {
          times.push_back(value);
        }
      }
    }
  }
  return times;
}

/**
 * Function returns the total number  of the processes.
 * @param[out] total number of processes
*/
long LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          long processes = 0;
          try {
            processes = std::stol(value);
          }
          catch (...) {
            processes = 0;
          }
          return processes;
        }
      }
    }
  }
  return 0;
}

/**
 * Function returns the number of running processes.
 * @param[out] number of running processes
*/
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          std::replace(value.begin(), value.end(), '_', ' ');
          int procsrunning = 0;
          try {
            procsrunning = std::stoi(value);
          }
          catch (...) {
            procsrunning = 0;
          }
          return procsrunning;
        }
      }
    }
  }
  return 0;
}

/**
 * Function returns the command which started the process with pid.
 * @param[in]  pid - process identifier
 * @param[out] command
*/
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line; 
}

/**
 * Function returns the size of memory with the process with pid.
 * @param[in]  pid - process identifier
 * @param[out] memory size in MB
*/
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;

  const string pidStr = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pidStr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return to_string(std::stol(value)/1000);
      }
    }
  }
  return string();
}

/**
 * Function returns the uptime in seconds of the process with pid.
 * @param[in]  pid - process identifier
 * @param[out] uid
*/
string LinuxParser::User(int pid) {
  string line;
  string key;
  string uid;

  const string pidStr = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pidStr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        string name;
        string password;
        string user;
        std::ifstream filestream(kPasswordPath);
        if (filestream.is_open()) {
          while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> name >> password >> user;
            if(user == uid) {
              return name;
            }  
          }
        }
      }
    }
  }
  return "no user found";
}

/**
 * Function returns the uptime in seconds of the process with pid.
 * @param[in]  pid - process identifier
 * @param[out] uptime in seconds
*/
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  long starttime = 0;

  const string pidStr = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pidStr + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++) {
      linestream >> value;
    }
  }
  try {
    starttime = std::stol(value);
  }
  catch (...) {
    starttime = 0;
  }
  return (UpTime() - starttime/sysconf(_SC_CLK_TCK));
}
