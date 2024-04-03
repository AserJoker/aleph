#pragma once
#include "../../core/Auto.hpp"
#include "../../core/Object.hpp"
#include <string>
namespace aleph::game {
class Interoperable {
public:
  virtual void action(const std::string &type,
                      const core::Auto<core::Object> &toucher) = 0;
};
} // namespace aleph::game