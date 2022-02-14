#include "tty.h"
#include "multiboot.h"
#include "kpanic.h"

#define SSFN_CONSOLEBITMAP_TRUECOLOR
// fix errors in ssfn.h using these macros
#define _STDINT_H
#include <scalable-font2/ssfn.h>

static uint32_t xpos;
static uint32_t ypos;
static uint32_t terminal_height = 0;
static uint32_t terminal_width = 0;
static uint32_t terminal_bpp = 0;
static volatile uint8_t *video = (uint8_t *) VIDEO; /// video memory

// font.o is linked against the kernel, with _binary_font_sfn_start symbol
extern char _binary_font_sfn_start;

#define VIDEO_MODE_VGA 0
#define VIDEO_MODE_VBE 1

static int vidoe_mode = VIDEO_MODE_VGA;

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

void init_tty(multiboot_info_t *mbi) {
  if (CHECK_FLAG(mbi->flags, 12)) {
    vidoe_mode = VIDEO_MODE_VBE;
  } else {
    vidoe_mode = VIDEO_MODE_VGA;
  }
  terminal_width = mbi->framebuffer_width;
  terminal_height = mbi->framebuffer_height;
  terminal_bpp = mbi->framebuffer_bpp;

  if (vidoe_mode == VIDEO_MODE_VBE)
    init_ssfn((uint8_t *) mbi->framebuffer_addr, mbi->framebuffer_width, mbi->framebuffer_height, mbi->framebuffer_bpp);
  // FrameBuffer
  //     fb(mbi->framebuffer_type, mbi->framebuffer_bpp, mbi->framebuffer_pitch, (uint8_t *) mbi->framebuffer_addr);
  // for (int i = 0; i < 500; ++i) {
  //   fb.put_pixel(i, i);
  // }
}

static void putchar_vga(char c) {
  if (c == '\n' || c == '\r') {
    newline:
    xpos = 0;
    ypos++;
    if (ypos >= terminal_height)
      ypos = 0;
    return;
  }

  *(video + (xpos + ypos * terminal_width) * 2) = c & 0xFF;
  *(video + (xpos + ypos * terminal_width) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= terminal_width)
    goto newline;
}

static void putchar_vbe(char c) {
  if (c == '\n' || c == '\r') {
    ssfn_dst.y += ssfn_src->height;
    ssfn_dst.x = 0;
  } else {
    ssfn_putc(c);
  }
}

void putchar(char c) {
  if (vidoe_mode == VIDEO_MODE_VBE) {
    putchar_vbe(c);
  } else if (vidoe_mode == VIDEO_MODE_VGA) {
    putchar_vga(c);
  }
}

/// Clear the screen and initialize VIDEO, XPOS and YPOS
void clear_screen() {
  if (vidoe_mode == VIDEO_MODE_VGA) {
    for (uint32_t i = 0; i < terminal_width * terminal_height * 2; i++)
      *(video + i) = 0;

    xpos = 0;
    ypos = 0;
  } else {
    kassert(terminal_bpp && terminal_width && terminal_height, "Invalid terminal screen size");
    for (uint32_t i = 0; i < terminal_width * terminal_height * terminal_bpp / 8; ++i) {
      ssfn_dst.ptr[i] = 0;
    }
  }
}

