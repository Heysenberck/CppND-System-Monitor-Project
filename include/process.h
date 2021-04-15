#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  void Pid(int id);
  int  Pid() const;
  void User(const std::string &usr);
  std::string User() const;                // TODO: See src/process.cpp
  void Command(const std::string &cmd);
  std::string Command() const;
  void CpuUtilization(long total_time);
  float CpuUtilization() const;
  void Ram(const std::string &mem);
  std::string Ram() const;
  void UpTime(long int time);
  long int UpTime() const;                 // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

private:
  int pid_;
  std::string command_;
  std::string ram_;
  std::string user_;
  long int uptime_;
  float cpuUtilization_;
};

#endif