#pragma once
#include "core/include/EventBase.hpp"
#include <cstdint>
#include <vector>
namespace aleph::system {
class InputEvent : public core::EventBase {
private:
  std::vector<int64_t> _codes;

public:
  InputEvent(const std::vector<int64_t> &codes);

  const std::vector<int64_t> &getCodes() const;

  static inline constexpr auto TYPE = "system.input";
};
}; // namespace aleph::system