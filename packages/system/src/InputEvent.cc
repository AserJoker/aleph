#include "system/include/InputEvent.hpp"
#include "core/include/EventBase.hpp"
#include <vector>
using namespace aleph;
using namespace aleph::system;
InputEvent::InputEvent(const std::vector<int> &codes)
    : core::EventBase("input"), _codes(codes) {}

const std::vector<int> &InputEvent::getCodes() const { return _codes; }