#pragma once
#include "../video/Size.hpp"
#include "Event.hpp"
namespace aleph::ui {
class EventResize : public Event {
private:
  video::Size size;

public:
  EventResize(const video::Size &size, Element *target)
      : Event("resize", target), size(size) {}
  const video::Size &getSize() const { return size; }
};
}; // namespace aleph::ui