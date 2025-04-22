#include "system/include/InputEvent.hpp"
#include "core/include/EventBase.hpp"
#include <cstdint>
#include <vector>
using namespace aleph;
using namespace aleph::system;
InputEvent::InputEvent(const std::vector<int64_t> &codes)
    : core::EventBase(TYPE), _codes(codes) {}

const std::vector<int64_t> &InputEvent::getCodes() const { return _codes; }