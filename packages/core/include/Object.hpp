#pragma once
#include "EventBase.hpp"
#include "core/include/ObjectBase.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace aleph::core {
class Object : public ObjectBase {
private:
  static std::unordered_map<std::string, std::vector<Object *>> _eventbus;

public:
  Object();

  ~Object() override;

  void emit(const EventBase &event);

  void emit(const std::string &event);

  virtual void onEvent(Object *emitter, const EventBase &event);

  void addEventListener(const std::string &event, Object *object);

  void removeEventListener(const std::string &event, Object *object);

  void removeEventListener();
};
} // namespace aleph::core