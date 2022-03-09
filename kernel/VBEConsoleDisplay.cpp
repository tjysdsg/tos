#include "VBEConsoleDisplay.h"
#include "kpanic.h"

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include "ssfn.h"

// font.o is linked against the kernel, with _binary_font_sfn_start symbol
extern char _binary_font_sfn_start;

/**
 * @brief Init scalable font renderer
 * @param framebuffer_ptr linear frame buffer address
 * @param width width in pixel, for example 1024
 * @param height height in pixel, for example 768
 * @param depth depth in bits, for example 32
 */
static void init_ssfn(uint8_t *framebuffer_ptr, int width, int height, int depth) {
  ssfn_dst.ptr = framebuffer_ptr;
  ssfn_src = (ssfn_font_t *) &_binary_font_sfn_start;

  ssfn_dst.w = width;
  ssfn_dst.h = height;
  ssfn_dst.p = (depth / 8) * width; // bytes per line
  ssfn_dst.x = ssfn_dst.y = 0;      // pen position
  ssfn_dst.fg = 0xFFFFFFFF;           // foreground color
  ssfn_dst.bg = 0xFF000000;
}

VBEConsoleDisplay::VBEConsoleDisplay(uint8_t *framebuffer_ptr, uint32_t width, uint32_t height, uint32_t depth)
    : ConsoleDisplay(width, height, depth) {
  _framebuffer = framebuffer_ptr;
  init_ssfn(_framebuffer, width, height, depth);

  _font_width = ssfn_src->width / 2;  // divide by 2 cuz the it's the pixel width of unicode character
  _font_height = ssfn_src->height;  // divide by 2 cuz the it's the pixel width of unicode character

  reset_cursor();
}

void VBEConsoleDisplay::putchar(char c) {
  switch (c) {
    case '\n':
      ssfn_dst.y += _font_height;
      _line_width[_lino++] = ssfn_dst.x;
    case '\r':
      ssfn_dst.x = 0;
      break;
    case '\b':
      if (ssfn_dst.x < _font_width) { // remove the last character of the previous line
        if (ssfn_dst.y == 0) break;

        --_lino;
        kassert(_lino, "Line number cannot be negative");

        ssfn_dst.y -= _font_height;
        ssfn_dst.x = _line_width[_lino] - _font_width;
        ssfn_putc(' ');
        ssfn_dst.x -= _font_width;
      } else {
        ssfn_dst.x -= _font_width;
        ssfn_putc(' ');
        ssfn_dst.x -= _font_width;
      }
      break;
    default:
      ssfn_putc(c);
      break;
  }
}

void VBEConsoleDisplay::clear_screen() {
  kassert(_depth && _width && _height, "Invalid terminal screen size");
  for (uint32_t i = 0; i < pixel_offset(_width, _height); ++i) {
    _framebuffer[i] = 0;
  }

  reset_cursor();
  kprintf("0x%x\n", &ssfn_dst.x);
}

void VBEConsoleDisplay::reset_cursor() {
  ssfn_dst.x = 0;
  ssfn_dst.y = 0;
}

uint32_t VBEConsoleDisplay::pixel_offset(uint32_t x, uint32_t y) {
  return ((_width * y + x) * _depth / 8);
}
