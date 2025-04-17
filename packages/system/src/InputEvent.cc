#include "system/include/InputEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
InputEvent::InputEvent(int32_t key) : core::EventBase("input"), _key(key) {}

int32_t InputEvent::getKey() const { return _key; }