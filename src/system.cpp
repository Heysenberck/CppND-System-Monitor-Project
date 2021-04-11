#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::unordered_map;

void System::Update() {
  ram_.Update();
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  unordered_map<int, string> userMap = LinuxParser::UserIdMap();
  std::vector<int> pids = LinuxParser::Pids();
  for(int id : pids) {
    Process process;
    process.Pid(id);
    process.Command(LinuxParser::Command(id));
    string uid = LinuxParser::User(id);
    string usr = userMap[std::stoi(uid)];
    process.Ram(LinuxParser::Ram(id));
    process.User(usr);
    process.UpTime(LinuxParser::UpTime(id));
    processes_.push_back(process);
  }
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return ram_.Utilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
