#pragma once
#include <string>
namespace aleph::core {
class EventBase {
private:
  std::string _type;

public:
  EventBase(const std::string &type);

  virtual ~EventBase() = default;

  const std::string &getType() const;
};
} // namespace aleph::core