#pragma once
#include "core/include/Event.hpp"
namespace aleph::system {
class WheelEvent : public core::Event<"system.wheel"> {
private:
  bool _direction;

public:
  WheelEvent(bool direction);

  bool getDirection() const;
};
}; // namespace aleph::system