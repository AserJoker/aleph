#pragma once
#include "Element.hpp"
#include "Event.hpp"
#include "EventChange.hpp"
#include "EventInput.hpp"
#include "EventKey.hpp"
#include <chrono>
#include <string>
namespace aleph::ui {
class Input : public Element {
private:
  std::wstring value;
  uint32_t max_size;
  int32_t cursor;
  bool edit;
  bool focus;

private:
  void submit() {
    EventChange change(value, this);
    emit(change);
  }
  void insert(const std::wstring &str) {
    auto part = str.substr(0, max_size - value.length());
    value.insert(cursor, part);
    setCursor(cursor + part.length());
    auto content = getContent();
    setSize({wcswidth(content.c_str(), content.length()), 1});
    EventInput input(value, this);
    emit(input);
  }
  void earse() {
    if (value.empty()) {
      return;
    }
    if (cursor == 0) {
      value.erase(0, 1);
    } else if (cursor == max_size - 1 && value.length() == max_size) {
      value.erase(value.length() - 1, 1);
    } else {
      value.erase(cursor - 1, 1);
      setCursor(cursor - 1);
    }
    EventInput input(value, this);
    emit(input);
  }

private:
  std::wstring getContent() {
    std::wstring str(max_size, L' ');
    str.replace(0, value.length(), value);
    return str;
  }
  void drawCursor(core::Auto<video::Renderer> &renderer) {
    using namespace std::chrono;
    static bool visible = true;
    static system_clock::time_point last = system_clock::now();
    if (system_clock::now() - last > 300ms) {
      visible = !visible;
      last = system_clock::now();
    }
    if (visible) {
      auto attr = getFontAttr();
      auto content = getContent();
      auto offstr = content.substr(0, cursor);
      renderer->draw(wcswidth(offstr.c_str(), offstr.length()), 0,
                     content[cursor], attr | device::Attribute::Standout);
    }
  }

protected:
  void on(Event &e) override {
    if (e.getEvent() == "key") {
      auto &ekey = (EventKey &)e;
      auto key = ekey.getKey();

      if (edit) {
        if (key.name == L"<enter>") {
          submit();
          edit = false;
        } else {
          if (key.name == L"<left>") {
            setCursor(cursor - 1);
          } else if (key.name == L"<right>") {
            setCursor(cursor + 1);
          } else if (key.name == L"<home>") {
            setCursor(0);
          } else if (key.name == L"<end>") {
            setCursor(max_size - 1);
          } else if (key.name == L"<esc>") {
            submit();
            edit = false;
          } else if (key.name == L"<backspace>") {
            earse();
          } else if (key.name == L"<tab>") {
            insert(L" ");
          } else if (key.visible) {
            insert(key.name);
          }
          e.cancel();
        }
      } else {
        if (key.name == L"<enter>") {
          edit = true;
        }
      }
    }
    if (e.getEvent() == "active") {
      focus = true;
    }
    if (e.getEvent() == "dective") {
      focus = false;
    }
    Element::on(e);
  }

public:
  Input(const uint32_t max_size = 13, const std::wstring &str = L"")
      : max_size(max_size), value(str), cursor(0), edit(false), focus(false) {
    setValue(str);
    setActivable(true);
  }
  void render(core::Auto<video::Renderer> &renderer) override {
    if (!edit) {
      auto attr = getFontAttr();
      attr | device::Attribute::Underline;
      if (focus) {
        attr | device::Attribute::Standout;
      }
      renderer->draw(0, 0, getContent(), attr);
    } else {
      auto attr = getFontAttr();
      attr | device::Attribute::Underline;
      auto content = getContent();
      renderer->draw(0, 0, content, attr);
      drawCursor(renderer);
    }
  }
  void setCursor(uint32_t cursor) {
    if (cursor < 0) {
      cursor = 0;
    }
    if (cursor > value.length()) {
      cursor = value.length();
    }
    if (cursor >= max_size) {
      cursor = max_size - 1;
    }
    this->cursor = cursor;
  }
  void setValue(const std::wstring &value) {
    this->value = value;
    int32_t width = wcswidth(value.c_str(), value.length());
    width += max_size - value.length();
    setSize({width, 1});
  }
  const std::wstring &getValue() const { return value; }
};
}; // namespace aleph::ui