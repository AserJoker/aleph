#include "system/include/KeyboardEvent.hpp"
#include "core/include/EventBase.hpp"
using namespace aleph;
using namespace aleph::system;
KeyboardEvent::KeyboardEvent(const std::wstring &key, bool shift, bool alt,
                             bool control)
    : core::EventBase("input"), _key(key), _shift(shift), _alt(alt),
      _control(control) {}

const std::wstring &KeyboardEvent::getKey() const { return _key; }