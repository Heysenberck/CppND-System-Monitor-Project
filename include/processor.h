#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

using std::vector;

class Processor {
 public:
  float Utilization();

 private:
  float prevIdle_{0.f};
  float currIdle_{0.f};
  float prevNonIdle_{0.f};
  float currNonIdle_{0.f};
  float prevTotal_{0.f};
  float currTotal_{0.f};
};

#endif