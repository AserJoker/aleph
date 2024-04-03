#pragma once
#include <cstdint>
namespace aleph::core {
class Object {
private:
  uint32_t ref;

public:
  const uint32_t &addRef() const {
    const_cast<uint32_t &>(ref)++;
    return ref;
  }
  const uint32_t &dispose() const {
    const_cast<uint32_t &>(ref)--;
    return ref;
  }
  Object() : ref(0) {}
  virtual ~Object() {}
  Object(const Object&) = delete;
};
} // namespace aleph::core
