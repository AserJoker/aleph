#pragma once
#include "Element.hpp"
#include "Event.hpp"
#include "EventChange.hpp"
#include "EventKey.hpp"
#include <map>
#include <string>
namespace aleph::ui {
class Select : public Element {
  class SelectLine : public Element {
  private:
    std::map<std::wstring, std::wstring> options;
    std::wstring value;
    std::wstring displayName;
    bool focus;

  public:
    SelectLine(const std::map<std::wstring, std::wstring> &options)
        : options(options), focus(false) {
      int32_t width = 0;
      for (auto &[displayName, _] : options) {
        width = std::max(width, wcswidth(displayName.c_str(),
                                                   displayName.length()));
      }
      setSize({width + 2, 1});
      setActivable(true);
    }
    void render(core::Auto<video::Renderer> &renderer) override {
      auto attr = getFontAttr();
      if (focus) {
        attr | device::Attribute::Standout;
      }
      renderer->draw(0, 0, L'[', attr);
      std::wstring content(getContentSize().width - 2, L' ');
      content.replace(0, displayName.length(), displayName);
      renderer->draw(1, 0, content, attr);
      renderer->draw(getContentSize().width - 1, 0, L']', attr);
    }
    void setFocus(bool focus) { this->focus = focus; }
    void setValue(const std::wstring &value) {
      this->value = value;
      for (auto &[displayName, value] : options) {
        if (value == this->value) {
          this->displayName = displayName;
        }
      }
    }
    const std::wstring &getValue() const { return this->value; }
  };
  class SelectPopup : public Element {
  private:
    std::map<std::wstring, std::wstring> options;
    std::wstring selected;

  public:
    void on(Event &event) override {
      if (event.getEvent() == "key") {
        EventKey &keyevent = (EventKey &)event;
        if (keyevent.getKey().name == L"<tab>") {
          if (selected.empty()) {
            selected = options.begin()->second;
          } else {
            auto it = options.begin();
            for (; it != options.end(); it++) {
              if (it->second == selected) {
                break;
              }
            }
            if (it == options.end()) {
              selected = options.begin()->second;
            } else {
              it++;
              if (it == options.end()) {
                selected = options.begin()->second;
              } else {
                selected = it->second;
              }
            }
          }
        }
      }
    }
    SelectPopup(const std::map<std::wstring, std::wstring> &options)
        : options(options) {
      int32_t width = 0;
      for (auto &[displayName, _] : options) {
        width = std::max(width, wcswidth(displayName.c_str(),
                                                   displayName.length()));
      }
      setSize({width, (int32_t)options.size()});
      ref() | ui::Element::Border();
    };
    void render(core::Auto<video::Renderer> &renderer) override {
      auto offset = 0;
      for (auto &[option, value] : options) {
        auto attr = getFontAttr();
        if (value == selected) {
          attr | device::Attribute::Standout;
        }
        renderer->draw(0, offset++, option, attr);
      }
    }
    void setSelected(const std::wstring &value) { this->selected = value; }
    const auto &getSelected() const { return selected; }
  };

private:
  bool expand;
  SelectLine *line;
  SelectPopup *popup;
  void onInput(EventKey &keyevent) {
    auto &key = keyevent.getKey();
    if (!expand) {
      if (key.name == L"<enter>") {
        expand = true;
        line->setFocus(false);
        popup->ref() | ui::Element::Margin(0);
        auto size = popup->getBoundSize();
        auto rootSize = getRoot()->getBoundSize();
        popup->ref() |
            ui::Element::Margin(rootSize.width / 2 - size.width / 2,
                                rootSize.height / 2 - size.height / 2);
        popup->setSelected(line->getValue());
        getRoot()->append(popup);
        popup->setVisible(true);
      }
    } else {
      if (key.name == L"<enter>") {
        expand = false;
        line->setFocus(true);
        line->setValue(popup->getSelected());
        popup->setVisible(false);
        EventChange change(line->getValue(), this);
        emit(change);
      } else {
        popup->on(keyevent);
      }
      keyevent.cancel();
    }
  }

protected:
  void on(Event &event) override {
    if (event.getEvent() == "active") {
      line->setFocus(true);
    } else if (event.getEvent() == "dective") {
      line->setFocus(false);
    } else if (event.getEvent() == "key") {
      auto &keyevent = (EventKey &)event;
      onInput(keyevent);
    }
    Element::on(event);
  }

public:
  Select(const std::map<std::wstring, std::wstring> &options,
         const std::wstring &value = L"")
      : line(nullptr), expand(false) {
    line = new SelectLine(options);
    line->setValue(value);
    popup = new SelectPopup(options);
    append(line);
    setSize(line->getBoundSize());
    setOverflowVisible(true);
    setActivable(true);
  }
  void setValue(const std::wstring &value) { line->setValue(value); }
  const std::wstring &getValue() { return line->getValue(); }
};
}; // namespace aleph::ui