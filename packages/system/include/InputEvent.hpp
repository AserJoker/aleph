#pragma once
#include "core/include/EventBase.hpp"
#include <vector>
namespace aleph::system {
class InputEvent : public core::EventBase {
private:
  std::vector<int> _codes;

public:
  InputEvent(const std::vector<int> &codes);
  
  const std::vector<int> &getCodes() const;
};
}; // namespace aleph::system