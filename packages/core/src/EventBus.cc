#include "core/include/EventBus.hpp"
#include "core/include/Object.hpp"
using namespace aleph;
using namespace aleph::core;

void EventBus::emit(Object *emitter, const BaseEvent &event) {
  for (auto &listeners : _bus[event.getType()]) {
    listeners->onEvent(emitter, event);
  }
}

void EventBus::addEventListener(const std::string &event, Object *object) {
  _bus[event].push_back(object);
}

void EventBus::removeEventListener(const std::string &event, Object *object) {
  auto &listeners = _bus[event];
  for (auto it = listeners.rbegin(); it != listeners.rend(); it++) {
    if (*it == object) {
      listeners.erase(it.base());
      return;
    }
  }
}

void EventBus::removeEventListener(Object *emitter) {
  for (auto &[_, bus] : _bus) {
    auto it = bus.begin();
    while (it != bus.end()) {
      if (*it == emitter) {
        bus.erase(it);
        it = bus.begin();
      } else {
        it++;
      }
    }
  }
}