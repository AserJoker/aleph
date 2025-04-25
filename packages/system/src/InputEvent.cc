#include "system/include/InputEvent.hpp"
#include <cstdint>
#include <vector>
using namespace aleph;
using namespace aleph::system;
InputEvent::InputEvent(const std::vector<int64_t> &codes) : _codes(codes) {}

const std::vector<int64_t> &InputEvent::getCodes() const { return _codes; }