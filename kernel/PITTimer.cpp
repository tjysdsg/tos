#include "PITTimer.h"
#include "pit.h"
#include "nop.h"

void PITTimer::prepare_sleep(uint32_t ms) {
  uint32_t tick = get_pit_tick();
  _target_tick = tick + ms * get_pit_freq() / 1000;

  // kprintf("pit_freq=%d\n", get_pit_freq());
  // kprintf("tick=%d, target_tick=%d\n", tick, _target_tick);
}

void PITTimer::do_sleep() {
  while (get_pit_tick() < _target_tick) { nop(); }
}
