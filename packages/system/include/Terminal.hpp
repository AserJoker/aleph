#pragma once
#include "core/include/Object.hpp"

namespace aleph::system {
class Terminal : public core::Object {
public:
  void setup();

  void cleanup();

  int readInput();

  void present();

  void move(int x, int y);

  void clear();

  void setCursor(bool visible);

  void setMouse(bool enable);

  void saveCursor();

  void loadCursor();
};
} // namespace aleph::system