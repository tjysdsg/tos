#include "tty.h"
#include "multiboot.h"

static int xpos;
static int ypos;
static volatile uint8_t *video = (unsigned char *) VIDEO; /// video memory


void putchar(char c) {
  if (c == '\n' || c == '\r') {
    newline:
    xpos = 0;
    ypos++;
    if (ypos >= TERM_LINES)
      ypos = 0;
    return;
  }

  *(video + (xpos + ypos * TERM_COLUMNS) * 2) = c & 0xFF;
  *(video + (xpos + ypos * TERM_COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= TERM_COLUMNS)
    goto newline;
}

/// Clear the screen and initialize VIDEO, XPOS and YPOS
void clear_screen() {
  int i;

  for (i = 0; i < TERM_COLUMNS * TERM_LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

