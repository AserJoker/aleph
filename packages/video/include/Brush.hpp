#pragma once
#include "core/include/ObjectBase.hpp"
#include "system/include/Terminal.hpp"
#include <string>
namespace aleph::video {
class Brush : public core::ObjectBase {
private:
  system::Terminal::Attr _attribute{};

  std::string _format;

  bool _updated{};

public:
  Brush(const system::Terminal::Attr &attr = {});

  const std::string &getFormat() const;

  const system::Terminal::Attr &getAttribute() const;

  void setAttribute(const system::Terminal::Attr &attribute);

  bool isUpdated() const;

  void setIsUpdated(bool updated);
};
} // namespace aleph::video