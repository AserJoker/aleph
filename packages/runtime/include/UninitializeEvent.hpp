#pragma once
#include "core/include/Event.hpp"
namespace aleph::runtime {
class UninitializeEvent : public core::Event<"runtime.uninitialize"> {};
}; // namespace aleph::runtime