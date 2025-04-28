#pragma once
#include "Brush.hpp"
#include "core/include/AutoPtr.hpp"
#include "core/include/Point.hpp"
#include "core/include/UString.hpp"
#include "runtime/include/Component.hpp"
namespace aleph::video {
class RenderableComponent : public runtime::Component<"RenderableComponent"> {
private:
  core::Point _position;

  core::AutoPtr<Brush> _brush;

  core::UString _ustring;

  std::string _mode{"ascii"};

public:
  const core::Point &getPosition() const;

  core::Point &getPosition();

  core::AutoPtr<Brush> &getBrush();

  const core::AutoPtr<Brush> &getBrush() const;

  const core::UString &getCharacter() const;

  void setCharacter(const std::string &source);
};
}; // namespace aleph::video