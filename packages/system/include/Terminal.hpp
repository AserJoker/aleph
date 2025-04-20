#pragma once
#include "core/include/Object.hpp"
namespace aleph::system {
class Terminal : public core::Object {
public:
  void setup();
  void cleanup();
  void pollEvent();
};
} // namespace aleph::system