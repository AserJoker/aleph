#pragma once
#include "core/include/Object.hpp"
#include <cstdint>
#include <unordered_map>

namespace aleph::system {
class Terminal : public core::Object {
private:
  std::unordered_map<uint32_t, short> _palette;
  std::unordered_map<uint32_t, short> _indices;

public:
  Terminal();

  ~Terminal() override;

  void pollEvent();

  void print(int x, int y, const char *msg);

  void printf(int x, int y, const char *fmt, auto... args) {
    char buf[1024];
    sprintf(buf, fmt, args...);
    print(x, y, buf);
  }

  void setColor(uint32_t fg, uint32_t bg);
};
} // namespace aleph::system