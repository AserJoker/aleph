#pragma once
#include "../core/Object.hpp"
#include "TerminalKey.hpp"
#include <curses.h>
namespace aleph::device {
class EventInput : public core::Object {
private:
  TerminalKey key;

public:
  const TerminalKey &getKey() const { return key; }
  EventInput(const TerminalKey &key) : key(key) {}
};
} // namespace aleph::device