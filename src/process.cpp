#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Pid(int processId) {
  pid_ = processId;
}

int Process::Pid() const { 
  return pid_;
}

void Process::CpuUtilization(float utilization) {
  long total_time = LinuxParser::ActiveJiffies(pid_);
  long seconds    = UpTime();
  float total_time_seconds = total_time / sysconf(_SC_CLK_TCK);
  utilization     = total_time_seconds / seconds;
  cpuUtilization_ = utilization;
}

float Process::CpuUtilization() const {
  return cpuUtilization_;
}

void Process::Command(const std::string &cmd) {
  command_ = cmd;
}

string Process::Command() const { 
  return command_;
}

void Process::Ram(const std::string &mem) {
  ram_ = mem;
}

string Process::Ram() const {
  return ram_;
}

void Process::User(const std::string &usr) {
  user_ = usr;
}

string Process::User() const {
  return user_;
}

void Process::UpTime(long int time) {
  uptime_ = time;
}

long int Process::UpTime() const {
  return uptime_;
}

bool Process::operator<(Process const& a) const {
  return cpuUtilization_ > a.CpuUtilization();
}