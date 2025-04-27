#pragma once
#include "Color.hpp"
#include "core/include/Color.hpp"
#include "core/include/Object.hpp"
#include "core/include/Point.hpp"
#include "core/include/Size.hpp"
#include <vector>

namespace aleph::system {
class Terminal : public core::Object {
public:
  struct Attr {
    core::Color background;
    core::Color color;
    uint16_t flag;
    constexpr static inline auto BOLD = 1 << 0;
    constexpr static inline auto HALF_BRIGHT = 1 << 1;
    constexpr static inline auto ITALIC = 1 << 2;
    constexpr static inline auto UNDERLINE = 1 << 4;
    constexpr static inline auto BLINK = 1 << 5;
    constexpr static inline auto COLOR = 1 << 6;
    constexpr static inline auto BACKGROUND = 1 << 7;
  };

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

  void flush();

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

  void setDecMode(bool enable);

  void print(const std::string &str);

  std::string compile(const Attr &attr);
};
} // namespace aleph::system