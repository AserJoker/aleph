#pragma once
#include "EventBase.hpp"
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
  using Callback = std::function<void(Object *, const EventBase &)>;

  std::unordered_map<std::string, std::vector<Callback>> _callbacks;

protected:
  core::Singleton<EventBus> _bus;

public:
  Object();

  ~Object() override;

  void onEvent(Object *emitter, const EventBase &event);

  void emit(const EventBase &event);

  template <class T, class... Args> void emit(Args... args) {
    T event{args...};
    emit(event);
  }

  template <class T, class E>
  void on(void (T::*callback)(Object *, const E &)) {
    auto &callbacks = _callbacks[typeid(E).name()];
    callbacks.push_back([this, callback](Object *e, const EventBase &ee) {
      (((T *)this)->*callback)(e, (const E &)ee);
    });
    _bus->addEventListener(typeid(E).name(), this);
  }
};
} // namespace aleph::core