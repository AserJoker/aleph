#pragma once
#include <cstdint>
#include <typeinfo>
#include <unordered_map>
namespace aleph::core {
class ObjectBase {
public:
  using Handle = uint64_t;

private:
  uint64_t _ref;

  Handle _handle;

private:
  static std::unordered_map<Handle, ObjectBase *> _tables;

  ObjectBase(const ObjectBase &) = delete;

public:
  ObjectBase();

  virtual ~ObjectBase();

  inline void addRef() { ++_ref; }

  inline void subRef() { --_ref; }

  inline uint64_t ref() const { return _ref; }

  inline Handle getHandle() const { return _handle; }

  inline virtual const char *getType() const { return typeid(*this).name(); }

  static ObjectBase *select(Handle handle);
};
} // namespace aleph::core