#pragma once
#include "core/include/Object.hpp"
namespace aleph::runtime {
class Module : public core::Object {
public:
  virtual void run() = 0;
};
}; // namespace aleph::runtime