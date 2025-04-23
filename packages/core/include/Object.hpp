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
  std::unordered_map<
      std::string,
      std::vector<std::function<void(Object *, const EventBase &)>>>
      _callbacks;

protected:
  core::AutoPtr<EventBus> _bus = core::Singleton<EventBus>::get();

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
    auto &callbacks = _callbacks[E::TYPE];
    callbacks.push_back([this, callback](Object *e, const EventBase &ee) {
      (((T *)this)->*callback)(e, (const E &)ee);
    });
    _bus->addEventListener(E::TYPE, this);
  }
};
} // namespace aleph::core