#include "system/include/ButtonEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
ButtonEvent::ButtonEvent(uint32_t button, bool status, bool shift, bool control,
                         bool alt)
    : core::EventBase(TYPE), _button(button), _status(status), _shift(shift),
      _control(control), _alt(alt) {}

uint32_t ButtonEvent::getButton() const { return _button; }

bool ButtonEvent::getStatus() const { return _status; }

bool ButtonEvent::isShift() const { return _shift; }

bool ButtonEvent::isControl() const { return _control; }

bool ButtonEvent::isAlt() const { return _alt; }