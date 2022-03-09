#include "VGAConsoleDisplay.h"

#define VGA_CHAR_ATTRIBUTE 7
#define VGA_VIDEO 0xB8000

VGAConsoleDisplay::VGAConsoleDisplay(uint32_t width, uint32_t height, uint32_t depth)
    : ConsoleDisplay(width, height, depth) {
  _video_addr = (uint8_t *) VGA_VIDEO;
}

void VGAConsoleDisplay::newline() {
  _x = 0;
  _y++;
  if (_y >= _height)
    _y = 0;
}

void VGAConsoleDisplay::putchar(char c) {
  if (c == '\n' || c == '\r') {
    newline();
    return;
  }

  *(_video_addr + (_x + _y * _width) * 2) = c & 0xFF;
  *(_video_addr + (_x + _y * _width) * 2 + 1) = VGA_CHAR_ATTRIBUTE;

  _x++;
  if (_x >= _width)
    newline();
}

void VGAConsoleDisplay::clear_screen() {
  for (uint32_t i = 0; i < _width * _height * 2; i++)
    *(_video_addr + i) = 0;

  _x = 0;
  _y = 0;
}
