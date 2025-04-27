#pragma once
#include "Event.hpp"
#include "EventBus.hpp"
#include "ObjectBase.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/Singleton.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace aleph::core {
class Object : public ObjectBase {
private:
  using Callback = std::function<void(Object *, const BaseEvent &)>;

  std::unordered_map<std::string, std::vector<Callback>> _callbacks;

protected:
  core::Singleton<EventBus> _bus;

public:
  Object();

  ~Object() override;

  void onEvent(Object *emitter, const BaseEvent &event);

  void emit(const BaseEvent &event);

  template <class T, class... Args> void emit(Args... args) {
    T event{args...};
    emit(event);
  }

  template <class T, class E>
  void on(void (T::*callback)(Object *, const E &)) {
    auto &callbacks = _callbacks[E::TYPE_NAME];
    callbacks.push_back([this, callback](Object *e, const BaseEvent &ee) {
      (((T *)this)->*callback)(e, (const E &)ee);
    });
    _bus->addEventListener(E::TYPE_NAME, this);
  }
};
} // namespace aleph::core