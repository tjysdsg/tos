#ifndef TOS_KERNEL_VBECONSOLEDISPLAY_H
#define TOS_KERNEL_VBECONSOLEDISPLAY_H
#include "ConsoleDisplay.h"
#include <stdint.h>

class VBEConsoleDisplay : public ConsoleDisplay {
 public:
  VBEConsoleDisplay() = delete;
  VBEConsoleDisplay(uint8_t *framebuffer_ptr, uint32_t width, uint32_t height, uint32_t depth);
  void putchar(char c) override;
  void clear_screen() override;
};

#endif //TOS_KERNEL_VBECONSOLEDISPLAY_H
