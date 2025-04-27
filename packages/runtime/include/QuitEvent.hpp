#pragma once
#include "core/include/Event.hpp"
namespace aleph::runtime {
class QuitEvent : public core::Event<"runtime.quit"> {
public:
};
} // namespace aleph::runtime