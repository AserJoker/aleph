#pragma once
#include "../core/Object.hpp"
#include <string>
namespace aleph::ui {
class Element;
class Event : public core::Object {
private:
  std::string event;
  Element *target;
  bool canceled;

public:
  Event(const std::string &event, Element *target)
      : event(event), target(target), canceled(false) {}
  const std::string &getEvent() const { return event; }
  const Element *getTarget() const { return target; }
  void cancel() { canceled = true; }
  bool isCanceled() const { return canceled; }
};
}; // namespace aleph::ui
