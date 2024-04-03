#pragma once
#include "../../video/Point.hpp"
namespace aleph::game {
class Movable {
public:
  virtual void move(const video::Point d) = 0;
};
}; // namespace aleph::game