
#pragma once
#include "Event.hpp"
namespace aleph::ui {
class EventDective : public Event {
public:
  EventDective(Element *target) : Event("dective", target) {}
};
}; // namespace aleph::ui