#include "system/include/KeyboardEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
KeyboardEvent::KeyboardEvent(int key)
    : core::EventBase("keyboard"), _key(key) {}

int KeyboardEvent::getKey() const { return _key; }