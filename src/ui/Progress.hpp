
#pragma once
#include "Element.hpp"
namespace aleph::ui {
class Progress : public Element {
private:
  uint32_t width;
  uint32_t value;

public:
  Progress(const int32_t &width = 20, const int32_t value = 0)
      : width(width), value(value) {
    setSize({width, 1});
  }
  void render(core::Auto<video::Renderer> &renderer) override {
    auto length = (value / 100.0f * width);
    for (auto i = 0; i < width; i++) {
      auto attr = getFontAttr();
      if (i < length) {
        renderer->draw(i, 0, L' ', attr | device::Attribute::Standout);
      } else {
        renderer->draw(i, 0, L' ', attr);
      }
    }
  }
  const uint32_t &getValue() const { return value; }
  void setValue(const uint32_t &value) { this->value = value; }
};
}; // namespace aleph::ui