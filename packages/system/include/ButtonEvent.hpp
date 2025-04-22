#pragma once
#include "core/include/EventBase.hpp"
#include <cstdint>
namespace aleph::system {
class ButtonEvent : public core::EventBase {
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

  static inline constexpr auto TYPE = "system.mouse";
};
}; // namespace aleph::system