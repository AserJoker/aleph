#pragma once
namespace aleph::core {
class EventBase {

public:
  EventBase();

  virtual ~EventBase() = default;

  virtual const char *getType() const;
};
} // namespace aleph::core