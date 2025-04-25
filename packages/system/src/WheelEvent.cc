#include "system/include/WheelEvent.hpp"
using namespace aleph;
using namespace aleph::system;
WheelEvent::WheelEvent(bool direction) : _direction(direction) {}

bool WheelEvent::getDirection() const { return _direction; }