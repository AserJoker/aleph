#pragma once
#include "../core/Auto.hpp"
#include "../core/Object.hpp"
#include "../video/Renderer.hpp"
#include "../video/Size.hpp"
#include "Event.hpp"
#include "EventActive.hpp"
#include "EventDective.hpp"
#include "EventResize.hpp"
#include <algorithm>
#include <cstdint>
#include <cwchar>
#include <vector>
namespace aleph::ui {
class Element : public core::Object {
private:
  Element *parent;
  std::vector<Element *> children;

  video::Size size;

  device::Symbol border_left;
  device::Symbol border_right;
  device::Symbol border_top;
  device::Symbol border_bottom;
  device::Symbol border_left_top;
  device::Symbol border_left_bottom;
  device::Symbol border_right_top;
  device::Symbol border_right_bottom;

  device::Attribute font_attr;

  int32_t margin_left;
  int32_t margin_right;
  int32_t margin_top;
  int32_t margin_bottom;

  uint32_t padding_left;
  uint32_t padding_right;
  uint32_t padding_top;
  uint32_t padding_bottom;

  video::Point offset;

  bool overflow_visible;
  bool floating;

  bool base_row_top;
  bool base_col_left;

  bool activable;

  bool visible;

public:
  bool isDebug = false;
  Element(Element *parent = nullptr)
      : parent(nullptr), margin_left(0), margin_right(0), margin_top(0),
        margin_bottom(0), padding_left(0), padding_right(0), padding_top(0),
        padding_bottom(0), overflow_visible(false), floating(false),
        base_row_top(true), base_col_left(true),
        font_attr({COLOR_WHITE, COLOR_BLACK}), activable(false), visible(true),
        offset({0, 0}) {
    if (parent) {
      this->parent = parent;
      parent->children.push_back(this);
    }
  }

  virtual ~Element() {
    if (parent) {
      auto it =
          std::find(parent->children.begin(), parent->children.end(), this);
      if (it != parent->children.end()) {
        parent->children.erase(it);
      }
      parent = nullptr;
    }
    for (auto &c : children) {
      c->parent = NULL;
      delete c;
    }
  }

  void append(Element *c, Element *pos = nullptr) {
    if (c->parent) {
      auto it =
          std::find(c->parent->children.begin(), c->parent->children.end(), c);
      if (it != c->parent->children.end()) {
        c->parent->children.erase(it);
      }
      c->parent = nullptr;
    }
    c->parent = this;
    if (pos) {
      auto it = std::find(children.begin(), children.end(), pos);
      if (it != children.end()) {
        children.insert(it, c);
      } else {
        children.push_back(c);
      }
    } else {
      children.push_back(c);
    }
  }
  void remove(Element *c) {
    if (c->parent != this) {
      return;
    }
    auto it =
        std::find(c->parent->children.begin(), c->parent->children.end(), c);
    if (it != c->parent->children.end()) {
      c->parent->children.erase(it);
    }
    c->parent = nullptr;
  }

  Element *getParent() { return parent; }
  const Element *getParent() const { return parent; }

  Element *getRoot() {
    auto root = parent;
    if (!root) {
      return this;
    }
    while (root->parent) {
      root = root->parent;
    }
    return root;
  }

  const Element *getRoot() const {
    auto root = parent;
    if (!root) {
      return this;
    }
    while (root->parent) {
      root = root->parent;
    }
    return root;
  }

  std::vector<Element *> getChildren() { return children; }
  const std::vector<Element *> &getChildren() const { return children; }

  bool isActivable() { return activable; }
  void setActivable(bool activable) { this->activable = activable; }

  bool isVisible() { return visible; }
  void setVisible(bool visible) { this->visible = visible; }

  void focus() {
    if (parent && !floating && !parent->overflow_visible) {
      video::Rect rc = {0, 0, 0, 0};
      rc.width = getBoundSize().width;
      rc.height = getBoundSize().height;
      rc.x = margin_left;
      rc.y = margin_top;
      if (!base_col_left) {
        rc.x = parent->getContentSize().width - parent->padding_right -
               margin_right - rc.width;
      }
      if (!base_row_top) {
        rc.y = parent->getContentSize().height - parent->padding_bottom -
               margin_bottom - rc.height;
      }
      auto newrc = rc;
      if (newrc.x + newrc.width >= parent->size.width) {
        newrc.x = parent->size.width - newrc.width;
      }
      if (newrc.x < 0) {
        newrc.x = 0;
      }
      if (newrc.y + newrc.height >= parent->size.height) {
        newrc.y = parent->size.height - newrc.height;
      }
      if (newrc.y < 0) {
        newrc.y = 0;
      }
      parent->setOffset({newrc.x - rc.x, newrc.y - rc.y});
      parent->focus();
    }
  }

  void active() {
    EventActive act(this);
    focus();
    emit(act);
  }
  void dective() {
    EventDective dec(this);
    emit(dec);
  }

  virtual void on(Event &event) {}
  void emit(Event &event) {
    on(event);
    if (event.isCanceled()) {
      return;
    }
    if (parent) {
      parent->on(event);
    }
  }

  void setSize(const video::Size &size) {
    this->size = size;
    EventResize resize(size, this);
    emit(resize);
  }

  virtual video::Size getContentSize() { return this->size; }

  uint32_t getLeftBorderWidth() {
    auto l = 0;
    if (border_left.ch != 0) {
      l = std::max(l, wcwidth(border_left.ch));
    }
    if (border_left_top.ch != 0) {
      l = std::max(l, wcwidth(border_left_top.ch));
    }
    if (border_left_bottom.ch != 0) {
      l = std::max(l, wcwidth(border_left_bottom.ch));
    }
    return l;
  }

  uint32_t getRightBorderWidth() {
    auto r = 0;
    if (border_right.ch != 0) {
      r = std::max(r, wcwidth(border_right.ch));
    }
    if (border_right_top.ch != 0) {
      r = std::max(r, wcwidth(border_right_top.ch));
    }
    if (border_right_top.ch != 0) {
      r = std::max(r, wcwidth(border_right_bottom.ch));
    }
    return r;
  }

  uint32_t getTopBorderWidth() {
    if (border_top.ch != 0 || border_left_top.ch != 0 ||
        border_right_top.ch != 0) {
      return 1;
    }
    return 0;
  }

  uint32_t getBottomBorderWidth() {
    if (border_bottom.ch != 0 || border_left_bottom.ch != 0 ||
        border_right_bottom.ch != 0) {
      return 1;
    }
    return 0;
  }

  video::Size getBoundSize() {
    video::Size size = this->getContentSize();
    size.width += getLeftBorderWidth();
    size.width += getRightBorderWidth();
    size.height += getTopBorderWidth();
    size.height += getBottomBorderWidth();
    return size;
  }
  video::Rect getBoundRect() {
    video::Rect rc = {0, 0};
    rc.width = getBoundSize().width;
    rc.height = getBoundSize().height;
    if (base_col_left) {
      rc.x = margin_left;
    } else {
      rc.x = parent->getContentSize().width - parent->padding_right -
             margin_right - rc.width;
    }
    if (base_row_top) {
      rc.y = margin_top;
    } else {
      rc.y = parent->getContentSize().height - parent->padding_bottom -
             margin_bottom - rc.height;
    }
    return rc;
  }

  virtual void render(core::Auto<video::Renderer> &renderer) {
    for (auto c : children) {
      auto off = renderer->getOffset();
      auto vp = renderer->getViewport();
      if (c->floating) {
        renderer->setOffset({0, 0});
      }
      if (c->floating || overflow_visible) {
        renderer->setViewport();
      }
      c->draw(renderer);
      renderer->setViewport(vp);
      renderer->setOffset(off);
    }
  }
  void drawHorizonalScroll(core::Auto<video::Renderer> &renderer) {
    video::Size csize = size;
    for (auto &c : children) {
      auto rc = c->getBoundRect();
      if (rc.x + rc.width >= csize.width) {
        csize.width = rc.x + rc.width;
      }
      if (rc.y + rc.height >= csize.height) {
        csize.height = rc.y + rc.height;
      }
    }
    if (csize.width > size.width) {
      auto width = size.width - 2;
      width *= (size.width * 1.0f / csize.width);
      if (width < 1) {
        width = 1;
      }
      auto off = -offset.x * 1.0f / (csize.width - size.width) *
                 (size.width - 2 - width);
      auto attr = getFontAttr();
      attr | device::Attribute::Standout;
      renderer->draw(1, getBoundSize().height - 1, L'<', attr);
      for (auto x = 0; x < width; x++) {
        renderer->draw(x + off + 2, getBoundSize().height - 1, L' ', attr);
      }
      renderer->draw(getBoundSize().width - 2, getBoundSize().height - 1, L'>',
                     attr);
    }
  }
  void drawVerticalScroll(core::Auto<video::Renderer> &renderer) {
    video::Size csize = size;
    for (auto &c : children) {
      auto rc = c->getBoundRect();
      if (rc.x + rc.width >= csize.width) {
        csize.width = rc.x + rc.width;
      }
      if (rc.y + rc.height >= csize.height) {
        csize.height = rc.y + rc.height;
      }
    }
    if (csize.height > size.height) {
      auto height = size.height - 2;
      height *= (size.height * 1.0f / csize.height);
      if (height < 1) {
        height = 1;
      }
      auto off = -offset.y * 1.0f / (csize.height - size.height) *
                 (size.height - 2 - height);
      auto attr = getFontAttr();
      attr | device::Attribute::Standout;
      renderer->draw(getBoundSize().width - 1, 1, L'+', attr);
      for (auto y = 0; y < height; y++) {
        renderer->draw(getBoundSize().width - 1, y + off + 2, L' ', attr);
      }
      renderer->draw(getBoundSize().width - 1, getBoundSize().height - 2, L'+',
                     attr);
    }
  }
  void draw(core::Auto<video::Renderer> &renderer) {
    if (isDebug) {
      isDebug = false;
    }
    if (!visible) {
      return;
    }
    auto vp = renderer->getViewport();
    auto off = renderer->getOffset();
    auto oldvp = vp;
    auto oldoff = off;
    auto rc = getBoundSize();
    if (base_row_top) {
      off.y += margin_top;
    } else {
      if (parent) {
        off.y = vp.height + vp.y - parent->padding_right -
                getBoundSize().height - margin_top;
      }
    }
    if (base_col_left) {
      off.x += margin_left;
    } else {
      if (parent) {
        off.x = vp.width + vp.x - parent->padding_right - getBoundSize().width -
                margin_right;
      }
    }
    vp.x = off.x;
    vp.y = off.y;
    vp.width = rc.width;
    vp.height = rc.height;
    renderer->setViewport(vp, true);
    renderer->setOffset(off);
    auto contentSize = getContentSize();
    if (border_left_top.ch) {
      renderer->draw(0, 0, border_left_top);
    }
    if (border_right_top.ch) {
      renderer->draw(contentSize.width + 1, 0, border_right_top);
    }
    if (border_left_bottom.ch) {
      renderer->draw(0, contentSize.height + 1, border_left_bottom);
    }
    if (border_right_bottom.ch) {
      renderer->draw(contentSize.width + 1, contentSize.height + 1,
                     border_right_bottom);
    }
    if (border_top.ch) {
      for (auto x = 1; x <= contentSize.width; x++) {
        renderer->draw(x, 0, border_top);
      }
    }
    if (border_bottom.ch) {
      for (auto x = 1; x <= contentSize.width; x++) {
        renderer->draw(x, contentSize.height + 1, border_top);
      }
      drawHorizonalScroll(renderer);
    }
    if (border_left.ch) {
      for (auto y = 1; y <= contentSize.height; y++) {
        renderer->draw(0, y, border_left);
      }
    }
    if (border_right.ch) {
      for (auto y = 1; y <= contentSize.height; y++) {
        renderer->draw(contentSize.width + 1, y, border_left);
      }
      drawVerticalScroll(renderer);
    }
    off.x += getLeftBorderWidth();
    off.y += getTopBorderWidth();
    vp.x = off.x;
    vp.y = off.y;
    vp.width -= getLeftBorderWidth();
    vp.width -= getRightBorderWidth();
    vp.height -= getTopBorderWidth();
    vp.height -= getBottomBorderWidth();

    off.x += padding_left;
    off.y += padding_top;

    off.x += offset.x;
    off.y += offset.y;
    renderer->setOffset(off);
    renderer->setViewport(vp, true);
    render(renderer);
    renderer->setViewport(oldvp);
    renderer->setOffset(oldoff);
  }

  void setLeftBorder(const device::Symbol &left) { border_left = left; }
  void setRightBorder(const device::Symbol &right) { border_right = right; }
  void setTopBorder(const device::Symbol &top) { border_top = top; }
  void setBottomBorder(const device::Symbol &bottom) { border_bottom = bottom; }
  void setLeftTopBorder(const device::Symbol &left_top) {
    border_left_top = left_top;
  }
  void setLeftBottomBorder(const device::Symbol &left_bottom) {
    border_left_bottom = left_bottom;
  }
  void setRightTopBorder(const device::Symbol &right_top) {
    border_right_top = right_top;
  }
  void setRightBottomBorder(const device::Symbol &right_bottom) {
    border_right_bottom = right_bottom;
  }
  void setOffset(const video::Point &off) {
    offset.x = off.x;
    offset.y = off.y;
  }

  const device::Symbol &getLeftBorder() const { return this->border_left; }
  const device::Symbol &getRightBorder() const { return this->border_right; }
  const device::Symbol &getTopBorder() const { return this->border_top; }
  const device::Symbol &getBottomBorder() const { return this->border_bottom; }
  const device::Symbol &getLeftTopBorder() const {
    return this->border_left_top;
  }
  const device::Symbol &getLeftBottomBorder() const {
    return this->border_left_bottom;
  }
  const device::Symbol &getRightTopBorder() const {
    return this->border_right_top;
  }
  const device::Symbol &getRightBottomBorder() const {
    return this->border_right_bottom;
  }
  const video::Point &getOffset() const { return offset; }

  void setBaseColLeft(bool left) { base_col_left = left; }
  void setBaseRowTop(bool top) { base_row_top = top; }

  void setLeftPadding(uint32_t padding) { padding_left = padding; }
  void setRightPadding(uint32_t padding) { padding_right = padding; }
  void setTopPadding(uint32_t padding) { padding_top = padding; }
  void setBottomPadding(uint32_t padding) { padding_bottom = padding; }

  const uint32_t &getLeftPadding() const { return padding_left; }
  const uint32_t &getRightPadding() const { return padding_right; }
  const uint32_t &getTopPadding() const { return padding_top; }
  const uint32_t &getBottomPadding() const { return padding_bottom; }

  void setLeftMargin(int32_t margin) { margin_left = margin; }
  void setRightMargin(int32_t margin) { margin_right = margin; }
  void setTopMargin(int32_t margin) { margin_top = margin; }
  void setBottomMargin(int32_t margin) { margin_bottom = margin; }

  const int32_t &getLeftMargin() const { return margin_left; }
  const int32_t &getRightMargin() const { return margin_right; }
  const int32_t &getTopMargin() const { return margin_top; }
  const int32_t &getBottomMargin() const { return margin_bottom; }

  void setOverflowVisible(bool visible) { overflow_visible = visible; }
  const bool &getOverflowVisible() const { return overflow_visible; }

  void setFloating(bool floating) { this->floating = floating; }
  const bool &getFloating() const { return floating; }

  void setFontAttr(const device::Attribute &attr) { this->font_attr = attr; }
  const device::Attribute &getFontAttr() const { return font_attr; }

  Element &ref() { return *this; }

  template <class T> Element &operator|(T fn) {
    fn(*this);
    return *this;
  }

public:
  static auto BorderLeft(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setLeftBorder(symbol); };
  }

  static auto BorderRight(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setRightBorder(symbol); };
  }

  static auto BorderTop(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setTopBorder(symbol); };
  }

  static auto BorderBottom(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setBottomBorder(symbol); };
  }

  static auto BorderLeftTop(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setLeftTopBorder(symbol); };
  }

  static auto BorderLeftBottom(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setLeftBottomBorder(symbol); };
  }

  static auto BorderRightTop(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setRightTopBorder(symbol); };
  }

  static auto BorderRightBottom(const device::Symbol &symbol) {
    return [=](Element &ele) -> void { ele.setRightBottomBorder(symbol); };
  }

  static auto BorderLeft(const device::Attribute &attr) {
    return BorderLeft({L'│', attr});
  }

  static auto BorderRight(const device::Attribute &attr) {
    return BorderRight({L'│', attr});
  }

  static auto BorderTop(const device::Attribute &attr) {
    return BorderTop({L'─', attr});
  }

  static auto BorderBottom(const device::Attribute &attr) {
    return BorderBottom({L'─', attr});
  }

  static auto BorderLeftTop(const device::Attribute &attr) {
    return BorderLeftTop({L'┌', attr});
  }

  static auto BorderLeftBottom(const device::Attribute &attr) {
    return BorderLeftBottom({L'└', attr});
  }

  static auto BorderRightTop(const device::Attribute &attr) {
    return BorderRightTop({L'┐', attr});
  }

  static auto BorderRightBottom(const device::Attribute &attr) {
    return BorderRightBottom({L'┘', attr});
  }

  static auto Border(const device::Attribute &attr) {
    return [=](Element &ele) -> void {
      ele | BorderLeft(attr) | BorderRight(attr) | BorderTop(attr) |
          BorderBottom(attr) | BorderLeftTop(attr) | BorderLeftBottom(attr) |
          BorderRightTop(attr) | BorderRightBottom(attr);
    };
  }

  static auto BorderLeft() {
    return [=](Element &ele) -> void { ele | BorderLeft(ele.font_attr); };
  }
  static auto BorderRight() {
    return [=](Element &ele) -> void { ele | BorderRight(ele.font_attr); };
  }
  static auto BorderTop() {
    return [=](Element &ele) -> void { ele | BorderTop(ele.font_attr); };
  }
  static auto BorderBottom() {
    return [=](Element &ele) -> void { ele | BorderBottom(ele.font_attr); };
  }
  static auto BorderLeftTop() {
    return [=](Element &ele) -> void { ele | BorderLeftTop(ele.font_attr); };
  }
  static auto BorderLeftBottom() {
    return [=](Element &ele) -> void { ele | BorderLeftBottom(ele.font_attr); };
  }
  static auto BorderRightTop() {
    return [=](Element &ele) -> void { ele | BorderRightTop(ele.font_attr); };
  }
  static auto BorderRightBottom() {
    return
        [=](Element &ele) -> void { ele | BorderRightBottom(ele.font_attr); };
  }
  static auto Border() {
    return [](Element &ele) -> void {
      ele | BorderLeft() | BorderRight() | BorderTop() | BorderBottom() |
          BorderLeftTop() | BorderLeftBottom() | BorderRightTop() |
          BorderRightBottom();
    };
  }
  static auto PaddingLeft(int32_t padding) {
    return [=](Element &ele) -> void { ele.setLeftPadding(padding); };
  }
  static auto PaddingRight(int32_t padding) {
    return [=](Element &ele) -> void { ele.setRightPadding(padding); };
  }
  static auto PaddingTop(int32_t padding) {
    return [=](Element &ele) -> void { ele.setTopPadding(padding); };
  }
  static auto PaddingBottom(int32_t padding) {
    return [=](Element &ele) -> void { ele.setBottomPadding(padding); };
  }
  static auto Padding(int32_t padding) {
    return [=](Element &ele) -> void {
      ele | PaddingLeft(padding) | PaddingRight(padding) | PaddingTop(padding) |
          PaddingBottom(padding);
    };
  }
  static auto Padding(int32_t paddingwidth, int32_t paddingheight) {
    return [=](Element &ele) -> void {
      ele | PaddingLeft(paddingwidth) | PaddingRight(paddingwidth) |
          PaddingTop(paddingheight) | PaddingBottom(paddingheight);
    };
  }
  static auto MarginLeft(int32_t margin) {
    return [=](Element &ele) -> void { ele.setLeftMargin(margin); };
  }
  static auto MarginRight(int32_t margin) {
    return [=](Element &ele) -> void { ele.setRightMargin(margin); };
  }
  static auto MarginTop(int32_t margin) {
    return [=](Element &ele) -> void { ele.setTopMargin(margin); };
  }
  static auto MarginBottom(int32_t margin) {
    return [=](Element &ele) -> void { ele.setBottomMargin(margin); };
  }
  static auto Margin(int32_t margin) {
    return [=](Element &ele) -> void {
      ele | MarginLeft(margin) | MarginRight(margin) | MarginTop(margin) |
          MarginBottom(margin);
    };
  }
  static auto Margin(int32_t marginwidth, int32_t marginheight) {
    return [=](Element &ele) -> void {
      ele | MarginLeft(marginwidth) | MarginRight(marginwidth) |
          MarginTop(marginheight) | MarginBottom(marginheight);
    };
  }
  static auto Size(int32_t width, int32_t height) {
    return [=](Element &ele) -> void { ele.setSize({width, height}); };
  }
  static auto FontColor(const device::Attribute &attr) {
    return [=](Element &ele) -> void { ele.setFontAttr(attr); };
  }
  static auto OverflowVisible(bool visible) {
    return [=](Element &ele) -> void { ele.setOverflowVisible(visible); };
  }
  static auto Floating(bool floating) {
    return [=](Element &ele) -> void { ele.setFloating(floating); };
  }
  static void BaseRowTop(Element &ele) { ele.setBaseRowTop(true); }
  static void BaseRowBottom(Element &ele) { ele.setBaseRowTop(false); }
  static void BaseColLeft(Element &ele) { ele.setBaseColLeft(true); }
  static void BaseColRight(Element &ele) { ele.setBaseColLeft(false); }

  static auto Visible(bool visible) {
    return [=](Element &ele) -> void { ele.setVisible(visible); };
  }
};
}; // namespace aleph::ui
