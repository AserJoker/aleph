#pragma once
#include "EventBase.hpp"
#include "ObjectBase.hpp"
#include <string>
#include <typeinfo>
#include <vector>
namespace aleph::core {
class Object;
class EventBus : public ObjectBase {
private:
  std::unordered_map<std::string, std::vector<Object *>> _bus;

public:
  void emit(Object *emitter, const EventBase &event);

  template <class T, class... Args> void emit(Object *emitter, Args... args) {
    T event{args...};
    emit(emitter, event);
  }

  void addEventListener(const std::string &event, Object *object);

  template <class T> void addEventListener(Object *object) {
    addEventListener(typeid(T).name(), object);
  }

  void removeEventListener(const std::string &event, Object *object);

  template <class T> void removeEventListener(Object *object) {
    removeEventListener(typeid(T).name(), object);
  }

  void removeEventListener(Object *emitter);
};
} // namespace aleph::core