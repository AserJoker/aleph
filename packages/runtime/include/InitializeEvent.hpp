#pragma once
#include "core/include/Event.hpp"
namespace aleph::runtime {
class InitializeEvent : public core::Event<"runtime.initialize"> {};
} // namespace aleph::runtime