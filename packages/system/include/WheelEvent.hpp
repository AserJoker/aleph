#pragma once
#include "core/include/EventBase.hpp"
namespace aleph::system {
class WheelEvent : public core::EventBase {
private:
  bool _direction;

public:
  WheelEvent(bool direction);

  bool getDirection() const;

  static inline constexpr auto TYPE = "system.wheel";
};
}; // namespace aleph::system