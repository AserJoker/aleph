#pragma once
#include "core/include/Event.hpp"
#include "core/include/Size.hpp"
namespace aleph::system {
class ResizeEvent : public core::Event<"system.resize"> {
private:
  core::Size _size;

public:
  ResizeEvent(const core::Size &size);

  const core::Size &getSize() const;
};
} // namespace aleph::system