#include "core/include/Object.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/ObjectBase.hpp"
using namespace aleph::core;

std::unordered_map<std::string, std::vector<Object *>> Object::_eventbus;

Object::Object() : ObjectBase() {}

Object::~Object() { removeEventListener(); }

void Object::emit(const EventBase &event) {
  for (auto &listeners : _eventbus[event.getType()]) {
    listeners->onEvent(this, event);
  }
}

void Object::emit(const std::string &event) {
  EventBase e = {event};
  for (auto &listeners : _eventbus[event]) {
    listeners->onEvent(this, e);
  }
}

void Object::onEvent(Object *, const EventBase &) {}

void Object::addEventListener(const std::string &event, Object *object) {
  _eventbus[event].push_back(object);
}

void Object::removeEventListener(const std::string &event, Object *object) {
  auto &listeners = _eventbus[event];
  for (auto it = listeners.rbegin(); it != listeners.rend(); it++) {
    if (*it == object) {
      listeners.erase(it.base());
      return;
    }
  }
}

void Object::removeEventListener() {
  for (auto &[_, bus] : _eventbus) {
    auto it = bus.begin();
    while (it != bus.end()) {
      if (*it == this) {
        bus.erase(it);
        it = bus.begin();
      } else {
        it++;
      }
    }
  }
}