#pragma once
#include "Point.hpp"
#include "Size.hpp"
#include <cstdint>
namespace aleph::core {
template <class P, class S> struct RectBase {
  union {
    PointBase<P> point;

    P x;

    P y;
  };

  union {
    SizeBase<S> size;

    S width;

    S height;
  };

  RectBase(P x = 0, P y = 0, S width = 0, S height = 0)
      : point(x, y), size(width, height) {}

  RectBase(const PointBase<P> &point, S width, S height)
      : point(point), size(width, height) {}

  RectBase(P x, P y, const SizeBase<S> &size) : point(x, y), size(size) {}

  RectBase(const PointBase<P> &point, const SizeBase<S> &size)
      : point(point), size(size) {}

  RectBase(const RectBase<P, S> &rect) : point(rect.point), size(rect.size) {}

  bool operator==(const RectBase<P, S> &another) const {
    return point == another.point && size == another.size;
  }
};

using Rect = RectBase<int32_t, uint32_t>;
} // namespace aleph::core