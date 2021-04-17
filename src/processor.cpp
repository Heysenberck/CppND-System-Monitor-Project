#include "processor.h"
#include "linux_parser.h"

#include <vector>
#include <string>
#include <iostream>

#include "linux_parser.h"

using std::vector;
using std::string;

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuTimeUnits = LinuxParser::CpuUtilization();

  float user       = std::stoi(cpuTimeUnits[CPUStates::kUser_]);
  float nice       = std::stoi(cpuTimeUnits[CPUStates::kNice_]);
  float system     = std::stoi(cpuTimeUnits[CPUStates::kSystem_]);
  float idle       = std::stoi(cpuTimeUnits[CPUStates::kIdle_]);
  float iowait     = std::stoi(cpuTimeUnits[CPUStates::kIOwait_]);
  float irq        = std::stoi(cpuTimeUnits[CPUStates::kIRQ_]);
  float softirq    = std::stoi(cpuTimeUnits[CPUStates::kSoftIRQ_]);
  float steal      = std::stoi(cpuTimeUnits[CPUStates::kSteal_]);

  prevIdle_ = currIdle_;
  currIdle_ = idle + iowait;

  prevNonIdle_ = currNonIdle_;
  currNonIdle_ = user + nice + system + irq + softirq + steal;

  float prevTotal = prevIdle_ + prevNonIdle_;
  float currTotal = currIdle_ + currNonIdle_;

  // differentiate: actual value minus the previous one
  float totald = currTotal - prevTotal;
  float idled  = currIdle_ - prevIdle_;

  return (totald - idled)/totald;
}
