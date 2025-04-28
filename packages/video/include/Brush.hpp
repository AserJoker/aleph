#pragma once
#include "core/include/ObjectBase.hpp"
#include "system/include/Terminal.hpp"
#include <string>
namespace aleph::video {
class Brush : public core::ObjectBase {
public:
  constexpr static inline uint8_t ASCII_MODE = 0;

  constexpr static inline uint8_t DEC_MODE = 1;

private:
  system::Terminal::Attr _attribute{};

  std::string _format;

  bool _updated{};

  uint8_t _mode{};

public:
  Brush(const system::Terminal::Attr &attr = {});

  const std::string &getFormat() const;

  const system::Terminal::Attr &getAttribute() const;

  void setAttribute(const system::Terminal::Attr &attribute);

  bool isUpdated() const;

  void setIsUpdated(bool updated);

  uint8_t getMode() const;

  void setMode(uint8_t mode);
};
} // namespace aleph::video