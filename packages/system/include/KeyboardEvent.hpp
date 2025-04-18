#pragma once
#include "core/include/EventBase.hpp"
namespace aleph::system {
class KeyboardEvent : public core::EventBase {
private:
  std::wstring _key;
  bool _shift;
  bool _alt;
  bool _control;

public:
  KeyboardEvent(const std::wstring &key, bool shift = false, bool alt = false,
                bool control = false);

  const std::wstring &getKey() const;
};
} // namespace aleph::system