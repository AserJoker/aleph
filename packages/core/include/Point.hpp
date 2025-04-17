#pragma once
#include <cstdint>
namespace aleph::core {
template <class T> struct PointBase {
  T x;

  T y;

  PointBase(T x = 0, T y = 0) : x(x), y(y) {}

  PointBase(const PointBase<T> &another) : x(another.x), y(another.y) {}

  bool operator==(const PointBase<T> &another) const {
    return x == another.x && y == another.y;
  }
};
using Point = PointBase<int32_t>;
} // namespace aleph::core