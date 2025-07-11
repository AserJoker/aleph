#pragma once
#include <string>
#include <vector>
namespace aleph::core {
class UString {

private:
  std::string _source;

  std::vector<size_t> _uindices;

  size_t offsetOf(size_t index) const;

  size_t charLength(size_t offset) const;

public:
  UString(const std::string &source = "");

  UString(const char *source);

  UString(const UString &another);

  size_t length() const;

  std::string at(size_t index) const;

  void set(size_t index, const std::string &chr);

  void erase(size_t begin, size_t end = -1);

  std::string &getSource();

  const std::string &getSource() const;

  size_t getRenderWidth() const;

  bool operator==(const UString &another) const;
};
}; // namespace aleph::core