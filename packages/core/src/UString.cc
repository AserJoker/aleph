#include "core/include/UString.hpp"
#include <wtswidth.h>
using namespace aleph;
using namespace aleph::core;
UString::UString(const std::string &source) : _source(source) {
  size_t idx = 0;
  for (size_t offset = 0; offset < _source.length(); offset = offsetOf(idx)) {
    _uindices.push_back(offset);
    idx++;
  }
}
UString::UString(const char *source) : _source(source) {
  size_t idx = 0;
  for (size_t offset = 0; offset < _source.length(); offset = offsetOf(idx)) {
    _uindices.push_back(offset);
    idx++;
  }
}
UString::UString(const UString &another) : _source(another._source) {
  size_t idx = 0;
  for (size_t offset = 0; offset < _source.length(); offset = offsetOf(idx)) {
    _uindices.push_back(offset);
    idx++;
  }
}

size_t UString::length() const { return _uindices.size(); }

std::string UString::at(size_t index) const {
  if (index < _uindices.size()) {
    auto begin = _uindices.at(index);
    if (index + 1 < _uindices.size()) {
      return {_source.begin() + begin, _source.begin() + _uindices[index + 1]};
    } else {
      return {_source.begin() + begin, _source.end()};
    }
  }
  return "";
};

void UString::set(size_t index, const std::string &chr) {
  if (index > _uindices.size()) {
    return;
  } else if (index == _uindices.size()) {
    _source += chr;
    return;
  }
  size_t offset = _uindices[index];
  if (index + 1 < _uindices.size()) {
    auto end = _uindices[index + 1];
    _source.replace(offset, end - offset, chr);
  } else {
    _source.replace(offset, _source.length() - offset, chr);
  }
  _uindices.clear();
  size_t idx = 0;
  for (size_t offset = 0; offset < _source.length(); offset = offsetOf(idx)) {
    _uindices.push_back(offset);
    idx++;
  }
}

void UString::erase(size_t begin, size_t end) {
  if (begin >= _uindices.size() || end <= begin) {
    return;
  }
  auto offset = _uindices[begin];
  if (end >= _uindices.size()) {
    _source.erase(_source.begin() + offset, _source.end());
  } else {
    auto next = _uindices[end];
    _source.erase(_source.begin() + offset, _source.begin() + next);
  }
  _uindices.clear();
  size_t idx = 0;
  for (size_t offset = 0; offset < _source.length(); offset = offsetOf(idx)) {
    _uindices.push_back(offset);
    idx++;
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
    offset += charLength(offset);
  }
  return offset;
}

size_t UString::charLength(size_t offset) const {
  if (offset >= _source.length()) {
    return _source.length();
  }
  auto ch = _source[offset];
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
    size += wts8width(chr.c_str(), chr.length());
  }
  return size;
}

bool UString::operator==(const UString &another) const {
  return _source == another._source;
}