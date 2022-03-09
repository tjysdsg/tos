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
  void reset_cursor();
  uint32_t pixel_offset(uint32_t x, uint32_t y);

 protected:
  uint8_t *_framebuffer = nullptr;
  int _line_width[1024]{};
  int _lino = 0;

  /// Assume using mono-width font
  int _font_width = 0;
  int _font_height = 0;
};

#endif //TOS_KERNEL_VBECONSOLEDISPLAY_H
