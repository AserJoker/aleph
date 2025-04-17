#pragma once
#include <cstdint>
namespace aleph::core {
template <class T> struct SizeBase {
  T width;

  T height;

  SizeBase(T width = 0, T height = 0) : width(width), height(height) {}

  SizeBase(const SizeBase<T> &another)
      : width(another.width), height(another.height) {}

  bool operator==(const SizeBase<T> &another) const {
    return width == another.width && height == another.height;
  }
};
using Size = SizeBase<uint32_t>;
} // namespace aleph::core