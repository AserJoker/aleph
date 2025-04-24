#pragma once
#include "Color.hpp"
#include "core/include/Color.hpp"
#include "core/include/Object.hpp"
#include "core/include/Point.hpp"
#include "core/include/Size.hpp"
#include <vector>

namespace aleph::system {
class Terminal : public core::Object {
private:
  core::Size _size;

  core::Point _mouse;

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

  void setNormal();

  void setBold(bool enable);

  void setHalfBright(bool enable);

  void setItalic(bool enable);

  void setUnderline(bool enable);

  void setBlink(bool enable);

  void setReverseVideo(bool enable);

  void setColor(const TERMINAL_COLOR_16 &color, bool bright = false);

  void setBackgroundColor(const TERMINAL_COLOR_16 &color, bool bright = false);

  void setColor(uint8_t color);

  void setBackgroundColor(uint8_t color);

  void setPalette(uint8_t idx, const core::Color &color);

  void setPalette(uint8_t idx, uint32_t color);

  void setColor(const core::Color &color);

  void setBackgroundColor(const core::Color &color);
};
} // namespace aleph::system