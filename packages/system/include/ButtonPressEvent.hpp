#pragma once
#include "core/include/EventBase.hpp"
namespace aleph::system {
class ButtonPressEvent : public core::EventBase {
private:
  short _button;
  bool _shift;
  bool _alt;
  bool _control;

public:
  ButtonPressEvent(short button, bool shift, bool alt, bool control);

  short getButton() const;

  bool isShiftPressed() const;

  bool isAltPressed() const;

  bool isControlPressed() const;
};
} // namespace aleph::system