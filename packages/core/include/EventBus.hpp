#pragma once
#include "Event.hpp"
#include "ObjectBase.hpp"
#include <string>
#include <vector>
namespace aleph::core {
class Object;
class EventBus : public ObjectBase {
private:
  std::unordered_map<std::string, std::vector<Object *>> _bus;

public:
  void emit(Object *emitter, const BaseEvent &event);

  template <class T, class... Args> void emit(Object *emitter, Args... args) {
    T event{args...};
    emit(emitter, event);
  }

  void addEventListener(const std::string &event, Object *object);

  template <class T> void addEventListener(Object *object) {
    addEventListener(T::TYPE_NAME, object);
  }

  void removeEventListener(const std::string &event, Object *object);

  template <class T> void removeEventListener(Object *object) {
    removeEventListener(T::TYPE_NAME, object);
  }

  void removeEventListener(Object *emitter);
};
} // namespace aleph::core