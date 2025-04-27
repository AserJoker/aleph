#include "core/include/Event.hpp"
#include <typeinfo>
using namespace aleph::core;

BaseEvent::BaseEvent() {}

const char *BaseEvent::getType() const { return typeid(*this).name(); }