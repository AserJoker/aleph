#include "system/include/KeyboardEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
KeyboardEvent::KeyboardEvent(int64_t key) : core::EventBase(TYPE), _key(key) {}

int64_t KeyboardEvent::getKey() const { return _key; }