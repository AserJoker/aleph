#include "core/include/UString.hpp"
#include <cstdint>
using namespace aleph;
using namespace aleph::core;
UString::UString(const std::string &source) : _source(source) {}
UString::UString(const char *source) : _source(source) {}
UString::UString(const UString &another) : _source(another._source) {}

size_t UString::length() const {
  size_t size = 0;
  for (size_t idx = 0; _source[idx] != 0;) {
    idx += charLength(idx);
    size++;
  }
  return size;
}

std::string UString::at(size_t index) const {
  std::string result;
  size_t offset = offsetOf(index);
  if (_source[offset] != 0) {
    auto begin = _source.data() + offset;
    result = {begin, begin + charLength(offset)};
  }
  return result;
};

void UString::set(size_t index, const std::string &chr) {
  size_t offset = offsetOf(index);
  if (_source[offset] != 0) {
    auto len = charLength(offset);
    _source.replace(offset, len, chr);
  }
}

std::string &UString::getSource() { return _source; }

const std::string &UString::getSource() const { return _source; }

size_t UString::offsetOf(size_t index) const {
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

size_t UString::charLength(size_t offset) const {
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

size_t UString::getRenderWidth() const {
  size_t size = 0;
  for (auto idx = 0; idx < length(); idx++) {
    auto chr = at(idx);
    if (chr.size() > 1) {
      size += 2;
    } else {
      size += 1;
    }
  }
  return size;
}

bool UString::operator==(const UString &another) const {
  return _source == another._source;
}