#pragma once
#include "core/include/Event.hpp"
namespace aleph::runtime {
class TickEvent : public core::Event<"runtime.tick"> {
public:
};
}; // namespace aleph::runtime