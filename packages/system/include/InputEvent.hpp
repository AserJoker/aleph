#pragma once
#include "core/include/EventBase.hpp"
#include <cstdint>
namespace aleph::system {
class InputEvent : public core::EventBase {
private:
  int32_t _key;

public:
  InputEvent(int32_t key);
  
  int32_t getKey() const;
};
} // namespace aleph::system