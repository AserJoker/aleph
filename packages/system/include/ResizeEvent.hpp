#pragma once
#include "core/include/EventBase.hpp"
#include "core/include/Size.hpp"
namespace aleph::system {
class ResizeEvent : public core::EventBase {
private:
  core::Size _size;

public:
  ResizeEvent(const core::Size &size);

  const core::Size &getSize() const;

  constexpr static inline auto TYPE = "system.resize";
};
} // namespace aleph::system