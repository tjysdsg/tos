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
  init_ssfn((uint8_t *) framebuffer_ptr, width, height, depth);
}

void VBEConsoleDisplay::putchar(char c) {
  if (c == '\n' || c == '\r') {
    ssfn_dst.y += ssfn_src->height;
    ssfn_dst.x = 0;
  } else {
    ssfn_putc(c);
  }
}

void VBEConsoleDisplay::clear_screen() {
  kassert(_depth && _width && _height, "Invalid terminal screen size");
  for (uint32_t i = 0; i < _width * _height * _depth / 8; ++i) {
    ssfn_dst.ptr[i] = 0;
  }

}

