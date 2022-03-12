#ifndef TOS_KERNEL_PITTIMER_H
#define TOS_KERNEL_PITTIMER_H

#include "Timer.h"

class PITTimer : public Timer {
 public:
  void prepare_sleep(uint32_t ms) override;
  void do_sleep() override;

 protected:
  uint32_t _target_tick = 0;
};

#endif //TOS_KERNEL_PITTIMER_H
