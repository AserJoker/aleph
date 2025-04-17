#include "system/include/WheelEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
WheelEvent::WheelEvent(bool direction)
    : core::EventBase("wheel"), _direction(direction) {}

bool WheelEvent::getDirection() const { return _direction; }