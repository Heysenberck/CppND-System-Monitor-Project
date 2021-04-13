#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

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

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return ram_ < a.ram_;
}