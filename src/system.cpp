#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {
  return cpu_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  std::vector<int> pids = LinuxParser::Pids();
  for( int id : pids ) {
    Process process;
    process.Pid(id);
    process.Command(LinuxParser::Command(id));
    process.Ram(LinuxParser::Ram(id));
    process.User(LinuxParser::User(id));
    process.UpTime(LinuxParser::UpTime(id));
    process.CpuUtilization(LinuxParser::ActiveJiffies(id));
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
  return LinuxParser::MemoryUtilization();
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

long System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
  return LinuxParser::UpTime();
}
