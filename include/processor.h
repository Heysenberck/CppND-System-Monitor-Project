#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  struct cpuTimeUnits {
    long user{0};
    long nice{0};
    long system{0};
    long idle{0};
    long iowait{0};
    long irq{0};
    long softirq{0};
    long steal{0};
    long guest{0};
    long guest_nice{0};
  };
  cpuTimeUnits prevCpuTimeUnits_;
  cpuTimeUnits currCpuTimeUnits_;

  float prevIdle_;
  float currIdle_;
  float prevNonIdle_;
  float currNonIdle_;
  float prevTotal_;
  float currTotal_;
};

#endif