#pragma once
#include "Auto.hpp"
#include "Object.hpp"
#include <functional>
#include <string>
#include <vector>
namespace aleph::core {
class EventBus : public Object {
private:
  struct Listener {
    Auto<Object> self;
    std::function<void(Auto<Object>, const Object &event)> handle;
    std::string event;
    uint32_t identity;
  };
  std::vector<Listener> listeners;

public:
  template <class T> void emit(const T &event) {
    auto name = typeid(T).name();
    for (auto &listener : listeners) {
      if (listener.event == name || listener.event == "*") {
        listener.handle(listener.self, event);
      }
    }
  }
  template <class T, class E>
  uint32_t on(const Auto<T> &self, void (T::*handle)(const E &)) {
    static uint32_t identity = 0;
    listeners.push_back(Listener{
        self,
        [=](Auto<Object> self, const Object &event) -> void {
          ((self.cast<T>().get())->*handle)(dynamic_cast<const E &>(event));
        },
        typeid(E).name(), ++identity});
    return identity;
  }
  template<class T,class E>
  uint32_t on(T *self,void(T::*handle)(const E&)){
    return this->on(Auto(self),handle);
  }
  void off(const uint32_t &identity) {
    for (auto it = listeners.begin(); it != listeners.end(); it++) {
      if (it->identity == identity) {
        listeners.erase(it);
        return;
      }
    }
  }
};
} // namespace aleph::core
