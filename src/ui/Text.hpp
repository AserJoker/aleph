#pragma once
#include "Element.hpp"
#include "Event.hpp"
#include <string>
namespace aleph::ui {
class Text : public Element {
private:
  std::wstring text;
  bool isActive;

protected:
  void on(Event &e) override {
    if (e.getEvent() == "active") {
      isActive = true;
    }
    if (e.getEvent() == "dective") {
      isActive = false;
    }
  }

public:
  Text(const std::wstring &text) : isActive(false) {
    setText(text);
    setActivable(true);
  }
  void setText(const std::wstring &text) {
    this->text = text;
    this->setSize({wcswidth(text.c_str(), text.length()), 1});
  }
  void render(core::Auto<video::Renderer> &renderer) override {
    auto attr = getFontAttr();
    if (isActive) {
      attr | device::Attribute::Standout;
    }
    renderer->draw(0, 0, text, attr);
  }
};
} // namespace aleph::ui