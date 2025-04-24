#include "system/include/ResizeEvent.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Size.hpp"
using namespace aleph;
using namespace aleph::system;
ResizeEvent::ResizeEvent(const core::Size &size)
    : core::EventBase(TYPE), _size(size) {}

const core::Size &ResizeEvent::getSize() const { return _size; }