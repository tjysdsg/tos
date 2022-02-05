#ifndef TOS_KERNEL_FRAMEBUFFER_H_
#define TOS_KERNEL_FRAMEBUFFER_H_
#include "multiboot.h"
#include <stdint.h>

class FrameBuffer {
 public:
  FrameBuffer() = delete;
  FrameBuffer(
      uint8_t framebuffer_type,
      uint8_t framebuffer_bpp,
      uint32_t framebuffer_pitch,
      uint8_t *framebuffer_addr
  );

  void set_color(int r, int g, int b);
  void put_pixel(int x, int y);

 private:
  uint8_t _framebuffer_type;
  uint8_t _framebuffer_bpp;
  uint32_t _framebuffer_pitch;
  uint8_t *_framebuffer_addr;

  uint32_t _color = 0xFFFFFFFF;
};

#endif //TOS_KERNEL_FRAMEBUFFER_H_
