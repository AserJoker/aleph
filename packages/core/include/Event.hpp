#pragma once
#include "Exception.hpp"
namespace aleph::core {
class BaseEvent {

public:
  BaseEvent();

  virtual ~BaseEvent() = default;

  virtual const char *getType() const;
};

template <TString type> class Event : public BaseEvent {
public:
  constexpr static auto TYPE_NAME = type.value;

  const char *getType() const override { return TYPE_NAME; };
};
} // namespace aleph::core