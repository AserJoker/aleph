#pragma once
#include "../../video/Renderer.hpp"
namespace aleph::aleph {
class Renderable {
public:
  virtual void render(const core::Auto<video::Renderer> &renderer) = 0;
};
}; // namespace aleph::aleph