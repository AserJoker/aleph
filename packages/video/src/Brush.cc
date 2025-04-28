#include "video/include/Brush.hpp"
#include "core/include/Singleton.hpp"
#include "system/include/Terminal.hpp"
using namespace aleph;
using namespace aleph::video;
const std::string &Brush::getFormat() const { return _format; }

const system::Terminal::Attr &Brush::getAttribute() const { return _attribute; }

void Brush::setAttribute(const system::Terminal::Attr &attribute) {
  _attribute = attribute;
  _format = core::Singleton<system::Terminal>::get()->compile(_attribute);
  _updated = true;
}

bool Brush::isUpdated() const { return _updated; }

void Brush::setIsUpdated(bool updated) { _updated = updated; }

uint8_t Brush::getMode() const { return _mode; }

void Brush::setMode(uint8_t mode) { _mode = mode; };

Brush::Brush(const system::Terminal::Attr &attr)
    : _attribute(attr), _updated(true) {
  _format = core::Singleton<system::Terminal>::get()->compile(_attribute);
}