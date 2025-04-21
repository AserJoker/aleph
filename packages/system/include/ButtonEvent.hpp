#pragma once
#include "core/include/EventBase.hpp"
namespace aleph::system {
class ButtonEvent : public core::EventBase {
private:
  int _button;
  
  bool _status;

  bool _shift;

  bool _control;

  bool _alt;

public:
  ButtonEvent(int button, bool status, bool shift = false, bool control = false,
              bool alt = false);

  int getButton() const;

  bool getStatus() const;

  bool isShift() const;

  bool isControl() const;

  bool isAlt() const;
};
}; // namespace aleph::system