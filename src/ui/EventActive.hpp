#pragma once
#include "Event.hpp"
namespace aleph::ui {
class EventActive : public Event {
public:
  EventActive(Element *target) : Event("active", target) {}
};
}; // namespace aleph::ui