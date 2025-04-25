#include "core/include/EventBase.hpp"
#include <typeinfo>
using namespace aleph::core;

EventBase::EventBase() {}

const char *EventBase::getType() const { return typeid(*this).name(); }