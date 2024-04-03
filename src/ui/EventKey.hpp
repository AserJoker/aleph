#pragma once
#include "../device/TerminalKey.hpp"
#include "Event.hpp"
namespace aleph::ui {
class EventKey : public Event {
private:
  TerminalKey key;

public:
  const TerminalKey &getKey() const { return key; }
  EventKey(const TerminalKey &key, Element *ele)
      : key(key), Event("key", ele) {}
};
} // namespace aleph::ui