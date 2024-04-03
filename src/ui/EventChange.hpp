#pragma once
#include "Element.hpp"
#include "Event.hpp"
#include <string>
namespace aleph::ui {
class EventChange : public Event {
private:
  std::wstring value;

public:
  EventChange(const std::wstring &value, Element *ele)
      : Event("change", ele), value(value) {}
  const std::wstring &getValue() const { return value; }
};
}; // namespace aleph::ui