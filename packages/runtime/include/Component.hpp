#pragma once
#include "core/include/Exception.hpp"
#include "core/include/ObjectBase.hpp"
namespace aleph::runtime {
class BaseComponent : public core::ObjectBase {};

template <core::TString type> class Component : public BaseComponent {
public:
  constexpr static inline auto TYPE_NAME = type.value;

public:
  const char *getType() const override { return type.value; }
};
}; // namespace aleph::runtime