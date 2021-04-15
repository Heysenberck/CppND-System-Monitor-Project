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


/**
 * Function returns the uptime of the system.
 * @param[out] uptime of the system
*/
long LinuxParser::UpTime() { 
  string line;
  long   uptime;
  long   idletime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
      return uptime; 
    }
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0; 
}

long LinuxParser::ActiveJiffies(int pid) {
  vector<string> values;  // [14=utime, 15=stime= 16=cutime, 17=cstime]
  long totalTime = 0;
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
  return stol(values[13]) + stol(values[14]) + stol(values[15]) + stol(values[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return 0;
}

// TODO: Read and return CPU utilization
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
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
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
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
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
  long uptime = 0;

  const string pidStr = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pidStr + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++) {
      linestream >> value;
    }
  }
  uptime = UpTime() - std::stol(value)/sysconf(_SC_CLK_TCK);
  return uptime;
}
