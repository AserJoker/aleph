#pragma once
#include "Color.hpp"
#include <cstdint>
#include <curses.h>
#include <unordered_map>
namespace aleph::device {
class Attribute {
private:
  Color front;
  Color background;
  uint32_t flag;
  int16_t identity;
  inline static std::unordered_map<uint32_t, int16_t> identitys;

public:
  Attribute(const Color front, const Color background)
      : front(front), background(background), flag(0) {
    uint32_t key = front.getIdentity();
    key <<= 16;
    key &= 0xffff0000;
    key |= background.getIdentity();
    static int16_t id = 0;
    if (identitys.contains(key)) {
      identity = identitys.at(key);
    } else {
      identity = ++id;
      identitys[key] = identity;
    }
  }
  template <class T> Attribute &operator|(T fn) {
    fn(this);
    return *this;
  }
  const attr_t getIdentity() const {
    int16_t f, b;
    pair_content(identity, &f, &b);
    if (f != front.getIdentity() || b != background.getIdentity()) {
      init_pair(identity, front.getIdentity(), background.getIdentity());
    }
    return COLOR_PAIR(identity) | flag;
  }

  static auto Underline(Attribute *attr) { attr->flag |= WA_UNDERLINE; }
  static auto Standout(Attribute *attr) { attr->flag |= WA_STANDOUT; }
  static auto Dim(Attribute *attr) { attr->flag |= WA_DIM; }
  static auto Bold(Attribute *attr) { attr->flag |= WA_BOLD; }
  static auto Italic(Attribute *attr) { attr->flag |= WA_ITALIC; }
};
} // namespace aleph::device