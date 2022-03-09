#ifndef TOS_KERNEL_CONSOLEDISPLAY_H
#define TOS_KERNEL_CONSOLEDISPLAY_H
#include <stdint.h>

class ConsoleDisplay {
 public:
  ConsoleDisplay() = delete;
  ConsoleDisplay(uint32_t width, uint32_t height, uint32_t depth) : _width(width), _height(height), _depth(depth) {
  }
  virtual ~ConsoleDisplay() = default;

  /**
   * @brief Put char at current cursor location
   */
  virtual void putchar(char c) = 0;

  /**
   * @brief Clear the entire screen and reset cursor location
   */
  virtual void clear_screen() = 0;

 protected:
  uint32_t _width = 0;
  uint32_t _height = 0;
  uint32_t _depth = 0;
};

#endif //TOS_KERNEL_CONSOLEDISPLAY_H
