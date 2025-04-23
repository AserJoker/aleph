#pragma once
#include <cstdint>
namespace aleph::core {
struct Color {
  uint8_t r;

  uint8_t g;

  uint8_t b;

  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  Color(uint32_t color = 0)
      : r((color >> 16) & 0xff), g((color >> 8) & 0xff), b(color & 0xff) {}
};
} // namespace aleph::core