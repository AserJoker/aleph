#pragma once
#include <cstdint>
#include <string>
namespace aleph::core {
class UString {

private:
  std::string _source;

  size_t offsetOf(size_t index) const {
    size_t offset = 0;
    std::string result;
    for (size_t idx = 0; idx < index; idx++) {
      if (!_source[offset]) {
        break;
      }
      auto ch = (uint8_t)_source[offset];
      if ((ch & 0xe0) == 0xc0) {
        offset += 2;
      } else if ((ch & 0xf0) == 0xe0) {
        offset += 3;
      } else if ((ch & 0xf8) == 0xf0) {
        offset += 4;
      } else if ((ch & 0xfc) == 0xf8) {
        offset += 5;
      } else if ((ch & 0xfe) == 0xfc) {
        offset += 6;
      } else {
        offset += 1;
      }
    }
    return offset;
  }

  size_t charLength(size_t offset) const {
    auto ch = (uint8_t)_source[offset];
    if ((ch & 0xe0) == 0xc0) {
      return 2;
    } else if ((ch & 0xf0) == 0xe0) {
      return 3;
    } else if ((ch & 0xf8) == 0xf0) {
      return 4;
    } else if ((ch & 0xfc) == 0xf8) {
      return 5;
    } else if ((ch & 0xfe) == 0xfc) {
      return 6;
    }
    return 1;
  }

public:
  UString(const std::string &source = "") : _source(source) {}

  UString(const char *source) : _source(source) {}

  UString(const UString &another) : _source(another._source) {}

  size_t length() const {
    size_t size = 0;
    for (size_t idx = 0; _source[idx] != 0;) {
      idx += charLength(idx);
      size++;
    }
    return size;
  }

  std::string at(size_t index) const {
    std::string result;
    size_t offset = offsetOf(index);
    if (_source[offset] != 0) {
      auto begin = _source.data() + offset;
      result = {begin, begin + charLength(offset)};
    }
    return result;
  };

  void set(size_t index, const std::string &chr) {
    size_t offset = offsetOf(index);
    if (_source[offset] != 0) {
      auto len = charLength(offset);
      _source.replace(offset, len, chr);
    }
  }

  std::string &getSource() { return _source; }

  const std::string &getSource() const { return _source; }
};
}; // namespace aleph::core