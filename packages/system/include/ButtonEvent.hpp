#pragma once
#include "core/include/Event.hpp"
#include <cstdint>
namespace aleph::system {
class ButtonEvent : public core::Event<"system.button"> {
private:
  uint32_t _button;

  bool _status;

  bool _shift;

  bool _control;

  bool _alt;

public:
  ButtonEvent(uint32_t button, bool status, bool shift = false,
              bool control = false, bool alt = false);

  uint32_t getButton() const;

  bool getStatus() const;

  bool isShift() const;

  bool isControl() const;

  bool isAlt() const;
};
}; // namespace aleph::system