#include "video/include/RenderableComponent.hpp"
#include "core/include/UString.hpp"
#include <cstring>
using namespace aleph;
using namespace aleph::video;
const core::Point &RenderableComponent::getPosition() const {
  return _position;
}

core::Point &RenderableComponent::getPosition() { return _position; }

core::AutoPtr<Brush> &RenderableComponent::getBrush() { return _brush; }

const core::AutoPtr<Brush> &RenderableComponent::getBrush() const {
  return _brush;
}

const core::UString &RenderableComponent::getCharacter() const {
  return _ustring;
}

void RenderableComponent::setCharacter(const std::string &source) {
  _ustring = source;
}