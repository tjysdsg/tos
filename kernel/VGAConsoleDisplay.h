#ifndef TOS_KERNEL_VGACONSOLEDISPLAY_H
#define TOS_KERNEL_VGACONSOLEDISPLAY_H

#include "ConsoleDisplay.h"
#include <stdint.h>

class VGAConsoleDisplay : public ConsoleDisplay {
 public:
  VGAConsoleDisplay() = delete;
  VGAConsoleDisplay(uint32_t width, uint32_t height, uint32_t depth);
  void putchar(char c) override;
  void clear_screen() override;

 protected:
  uint32_t _x = 0;
  uint32_t _y = 0;
  volatile uint8_t *_video_addr = nullptr; /// video memory

  void newline();
};

#endif //TOS_KERNEL_VGACONSOLEDISPLAY_H
