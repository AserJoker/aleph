#include "core/include/ObjectBase.hpp"
using namespace aleph::core;

std::unordered_map<ObjectBase::Handle, ObjectBase *> ObjectBase::_tables;

ObjectBase::ObjectBase() : _ref(0), _handle(0) {
  static Handle generator = 0;
  _handle = ++generator;
  _tables[_handle] = this;
}

ObjectBase::~ObjectBase() { _tables.erase(_handle); }

ObjectBase *ObjectBase::select(Handle handle) {
  if (!_tables.contains(handle)) {
    return nullptr;
  }
  return _tables.at(handle);
}