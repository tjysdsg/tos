#include "FrameBuffer.h"

void FrameBuffer::set_color(int r, int g, int b) {
  switch (_framebuffer_type) {
    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED: {
      // find the nearest color in the palette
      // FIXME:
      //  unsigned best_distance, distance;
      //  struct multiboot_color *palette;
      //  palette = (struct multiboot_color *) mbi->framebuffer_palette_addr;
      //  _color = 0;
      //  best_distance = 4 * 256 * 256;
      //  for (int i = 0; i < _framebuffer_palette_num_colors; i++) {
      //    distance = (0xff - palette[i].blue) * (0xff - palette[i].blue)
      //        + palette[i].red * palette[i].red
      //        + palette[i].green * palette[i].green;
      //    if (distance < best_distance) {
      //      _color = i;
      //      best_distance = distance;
      //    }
      //  }
      break;
    }
    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB: {
      // FIXME: _color = ((1 << _framebuffer_blue_mask_size) - 1) << _framebuffer_blue_field_position;
      break;
    }
    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT: {
      _color = '\\' | 0x0100;
      break;
    }
    default: {
      _color = 0xFFFFFFFF;
      break;
    }
  }
}

void FrameBuffer::put_pixel(int x, int y) {
  // TODO: boundary checking
  switch (_framebuffer_bpp) {
    case 8: {
      uint8_t *pixel = _framebuffer_addr + _framebuffer_pitch * x + y;
      *pixel = _color;
      break;
    }
    case 15:
    case 16: {
      auto *pixel = reinterpret_cast<uint16_t *>(_framebuffer_addr + _framebuffer_pitch * x + 2 * y);
      *pixel = _color;
    }
      break;
    case 24: {
      auto *pixel = reinterpret_cast<uint32_t *>(_framebuffer_addr + _framebuffer_pitch * x + 3 * y);
      *pixel = (_color & 0xFFFFFF) | (*pixel & 0xFF000000);
      break;
    }
    case 32: {
      auto *pixel = reinterpret_cast<uint32_t *>(_framebuffer_addr + _framebuffer_pitch * x + 4 * y);
      *pixel = _color;
      break;
    }
  }
}

FrameBuffer::FrameBuffer(uint8_t framebuffer_type,
                         uint8_t framebuffer_bpp,
                         uint32_t framebuffer_pitch,
                         uint8_t *framebuffer_addr)
    : _framebuffer_type(framebuffer_type),
      _framebuffer_bpp(framebuffer_bpp),
      _framebuffer_pitch(framebuffer_pitch),
      _framebuffer_addr(framebuffer_addr) {}
