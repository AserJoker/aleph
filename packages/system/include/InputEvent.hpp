#pragma once
#include "core/include/Event.hpp"
#include <cstdint>
#include <vector>
namespace aleph::system {
class InputEvent : public core::Event<"system.input"> {
private:
  std::vector<int64_t> _codes;

public:
  InputEvent(const std::vector<int64_t> &codes);

  const std::vector<int64_t> &getCodes() const;
};
}; // namespace aleph::system