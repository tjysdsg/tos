#include "tty.h"
#include "multiboot.h"
#include "kpanic.h"
#include "VBEConsoleDisplay.h"
#include "VGAConsoleDisplay.h"

static ConsoleDisplay *console_display = nullptr;

void init_tty(multiboot_info_t *mbi) {
  if (CHECK_FLAG(mbi->flags, 12)) {
    static VBEConsoleDisplay vbe_display(
        (uint8_t *) mbi->framebuffer_addr,
        mbi->framebuffer_width,
        mbi->framebuffer_height,
        mbi->framebuffer_bpp
    );
    console_display = &vbe_display;
  } else {
    static VGAConsoleDisplay vga_display(
        mbi->framebuffer_width,
        mbi->framebuffer_height,
        mbi->framebuffer_bpp
    );
    console_display = &vga_display;
  }
}

void putchar(char c) {
  kassert(console_display, "console_display is not initialized correctly");
  console_display->putchar(c);
}

void clear_screen() {
  console_display->clear_screen();
}

void tty_show_keyboard_input(char c) {
  kprintf("%c", c);
}

