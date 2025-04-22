#pragma once
#include "core/include/Object.hpp"
#include "core/include/Point.hpp"
#include "core/include/Size.hpp"
#include <vector>

namespace aleph::system {
class Terminal : public core::Object {
private:
  core::Size _size;

  core::Point _mouse;

  uint32_t _mouseStatus;

  std::vector<int64_t> _codes;

  int64_t readInput();

  void parseEvent();

public:
  Terminal();

  void setup();

  void cleanup();

  void pollEvent();

  void present();

  void move(int x, int y);

  void clear();

  void setCursor(bool visible);

  void setMouse(bool enable);

  void saveCursor();

  void loadCursor();

  const core::Size &getSize() const;

  const core::Point &getMousePosition() const;
};
} // namespace aleph::system