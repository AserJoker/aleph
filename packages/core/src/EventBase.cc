#include "core/include/EventBase.hpp"
using namespace aleph::core;

EventBase::EventBase(const std::string &event) : _type(event) {}

const std::string &EventBase::getType() const { return _type; }