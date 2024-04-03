#pragma once
#include <cstdint>
#include <curses.h>
namespace aleph::device {
class Color {
private:
  int16_t r;
  int16_t g;
  int16_t b;
  uint16_t identity;

public:
  Color(uint8_t r, uint8_t g, uint8_t b) {
    this->r = (int16_t)((r / 255.0f) * 1000);
    this->g = (int16_t)((g / 255.0f) * 1000);
    this->b = (int16_t)((b / 255.0f) * 1000);
    static uint16_t id = 16;
    identity = ++id;
  }
  Color(const uint16_t &id) {
    identity = id;
    color_content(id, &r, &g, &b);
  }
  const uint16_t getIdentity() const {
    short r, g, b;
    color_content(identity, &r, &g, &b);
    if (r != this->r || g != this->g || b != this->b) {
      init_color(identity, r, g, b);
    }
    return identity;
  }
};
}; // namespace aleph::device