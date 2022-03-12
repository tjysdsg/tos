#include "Timer.h"

void Timer::sleep(uint32_t ms) {
  prepare_sleep(ms);
  do_sleep();
}
