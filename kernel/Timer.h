#ifndef TOS_KERNEL_TIMER_H
#define TOS_KERNEL_TIMER_H
#include "stdint.h"

class Timer {
 public:
  Timer() = default;
  virtual ~Timer() = default;

  virtual void prepare_sleep(uint32_t ms) = 0;
  virtual void do_sleep() = 0;
  void sleep(uint32_t ms);
};

#endif //TOS_KERNEL_TIMER_H
