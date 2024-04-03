#pragma once
#include "Attribute.hpp"
namespace aleph::device {
struct Symbol {
  wchar_t ch;
  Attribute attr;
  Symbol(const wchar_t ch = 0,
         const Attribute &attr = {COLOR_WHITE, COLOR_BLACK}):ch(ch),attr(attr) {}
};
} // namespace aleph::device
