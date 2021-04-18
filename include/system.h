#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "memory.h"

class System {
public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  long TotalProcesses(); 
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

private:
  std::string kernel_;
  std::string operating_system_;
  Processor   cpu_;
  std::vector<Process> processes_ = {};
};

#endif