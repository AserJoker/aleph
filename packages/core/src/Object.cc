#include "core/include/Object.hpp"
#include "core/include/Event.hpp"
#include "core/include/EventBus.hpp"
#include "core/include/ObjectBase.hpp"
using namespace aleph::core;

Object::Object() : ObjectBase() {}

Object::~Object() { _bus->removeEventListener(this); }

void Object::onEvent(Object *emitter, const BaseEvent &event) {
  auto type = event.getType();
  auto &callbacks = _callbacks[type];
  for (auto &callback : callbacks) {
    callback(emitter, event);
  }
}

void Object::emit(const BaseEvent &event) { _bus->emit(this, event); }
