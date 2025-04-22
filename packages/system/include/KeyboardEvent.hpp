#pragma once
#include "core/include/EventBase.hpp"
#include <cstdint>
namespace aleph::system {
class KeyboardEvent : public core::EventBase {
private:
  int64_t _key;

public:
  KeyboardEvent(int64_t key);

  int64_t getKey() const;

  static inline constexpr auto TYPE = "system.keyboard";
};
}; // namespace aleph::system