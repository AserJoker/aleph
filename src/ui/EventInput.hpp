
#pragma once
#include "Element.hpp"
#include "Event.hpp"
namespace aleph::ui {
class EventInput : public Event {
private:
  std::wstring value;

public:
  EventInput(const std::wstring &value, Element *target)
      : Event("input", target), value(value) {}
};
}; // namespace aleph::ui