#pragma once
#include "../core/Object.hpp"
#include "../device/Terminal.hpp"
#include "Point.hpp"
#include "Rect.hpp"
#include <curses.h>
#include <string>
#include <unordered_map>
namespace aleph::video {
class Renderer : public core::Object {
private:
  struct Cell {
    int32_t x;
    int32_t y;
    device::Symbol symbol;
  };

  core::Auto<device::Terminal> terminal;
  device::Attribute defaultAttribute;
  Rect viewport;
  Point offset;
  std::unordered_map<std::string, Cell> cells;

public:
  Renderer(const core::Auto<device::Terminal> &terminal)
      : terminal(terminal), defaultAttribute(COLOR_WHITE, COLOR_BLACK),
        viewport({0, 0, 0, 0}), offset({0, 0}) {
    this->terminal->getTerminalSize(viewport.width, viewport.height);
  }
  void draw(int32_t x, int32_t y, wchar_t ch, const device::Attribute &attr) {
    int32_t vx = x + offset.x;
    int32_t vy = y + offset.y;
    if (vx >= viewport.x && vx < viewport.x + viewport.width &&
        vy >= viewport.y && vy < viewport.y + viewport.height) {
      // terminal->draw(vx, vy, {ch, attr});
      auto key = fmt::format("{},{}", vx, vy);
      cells[key] = {vx, vy, {ch, attr}};
    }
  }
  void present() { terminal->present(); }
  void draw(int32_t x, int32_t y, wchar_t ch) {
    draw(x, y, ch, defaultAttribute);
  }
  void draw(int32_t x, int32_t y, const device::Symbol &symbol) {
    auto &[ch, attr] = symbol;
    draw(x, y, ch, attr);
  }

  void draw(int32_t x, int32_t y, const std::wstring &str,
            const device::Attribute &attr) {
    auto offset = 0;
    for (auto &ch : str) {
      draw(x + offset, y, ch, attr);
      offset += wcwidth(ch);
    }
  }
  void draw(int32_t x, int32_t y, const std::wstring &str) {
    draw(x, y, str, defaultAttribute);
  }

  const Point &getOffset() const { return offset; }
  void setOffset(const Point &offset) { this->offset = offset; }
  const Rect &getViewport() const { return viewport; }
  void setViewport(const Rect &vpinput, bool fix = false) {
    auto vp = vpinput;
    if (fix) {
      if (vp.x < viewport.x) {
        vp.width -= viewport.x - vp.x;
        vp.x = viewport.x;
      }
      if (vp.y < viewport.y) {
        vp.height -= viewport.y - vp.y;
        vp.y = viewport.y;
      }
      if (vp.x + vp.width >= viewport.x + viewport.width) {
        vp.width = viewport.x + viewport.width - vp.x;
      }
      if (vp.y + vp.height >= viewport.y + viewport.height) {
        vp.height = viewport.y + viewport.height - vp.y;
      }
    }
    this->viewport = vp;
  }
  void setViewport() {
    int32_t w, h;
    terminal->getTerminalSize(w, h);
    this->viewport = {0, 0, w, h};
  }
  void clear() {
    auto oldoff = offset;
    auto oldvp = viewport;
    offset = {0, 0};
    int32_t w, h;
    terminal->getTerminalSize(w, h);
    viewport = {0, 0, w, h};
    for (auto x = 0; x < w; x++) {
      for (auto y = 0; y < h; y++) {
        draw(x, y, L' ');
      }
    }
    viewport = oldvp;
    offset = oldoff;
  }
  core::Auto<device::Terminal> &getTerminal() { return terminal; }
};
}; // namespace aleph::video
