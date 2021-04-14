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

Processor& System::Cpu() {
  return cpu_;
}

bool sortByCpuUtilization(const Process &a, const Process &b) {
  return a < b;
}

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
    float util = 0.f;
    process.CpuUtilization(util/*LinuxParser::CpuUtililization(id)*/);
    processes_.push_back(process);
  } 
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() {
  if( kernel_.empty()) {
    kernel_ = LinuxParser::Kernel();
  }
  return kernel_;
}

float System::MemoryUtilization() { 
  return ram_.Utilization();
}

std::string System::OperatingSystem() {
  if(operating_system_.empty()) {
    operating_system_ = LinuxParser::OperatingSystem();
  }
  return operating_system_;
}

int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
  return LinuxParser::UpTime();
}
