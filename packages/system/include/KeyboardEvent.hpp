#pragma once
#include "core/include/EventBase.hpp"
namespace aleph::system {
class KeyboardEvent : public core::EventBase {
private:
  int _key;

public:
  KeyboardEvent(int key);
  
  int getKey() const;
};
}; // namespace aleph::system