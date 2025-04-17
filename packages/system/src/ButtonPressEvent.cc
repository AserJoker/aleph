#include "core/include/EventBase.hpp"
#include "system/include/ButtonPressEvent.hpp"

using namespace aleph::system;
ButtonPressEvent::ButtonPressEvent(short button, bool shift, bool alt,
                                   bool control)
    : core::EventBase("button_press"), _button(button), _shift(shift),
      _alt(alt), _control(control) {}

short ButtonPressEvent::getButton() const { return _button; }

bool ButtonPressEvent::isShiftPressed() const { return _shift; }

bool ButtonPressEvent::isAltPressed() const { return _alt; }

bool ButtonPressEvent::isControlPressed() const { return _control; }