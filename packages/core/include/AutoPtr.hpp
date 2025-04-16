#pragma once
#include <cstdint>
#include <type_traits>
template <class T>
concept Ref = requires(T *obj) {
  { obj->addRef() };
  { obj->subRef() };
  { std::is_same_v<decltype(obj->ref()), uint64_t> };
};

namespace aleph::core {
template <Ref T> class AutoPtr {
private:
  T *_object;
  void addRef() {
    if (_object) {
      _object->addRef();
    }
  }
  void dispose() {
    if (_object) {
      _object->subRef();
      if (!_object->ref()) {
        delete _object;
      }
      _object = nullptr;
    }
  }

public:
  AutoPtr(T *raw = nullptr) : _object(raw) { addRef(); }

  AutoPtr(const AutoPtr<T> &another)
      : _object(const_cast<T *>(another._object)) {
    addRef();
  }

  virtual ~AutoPtr() { dispose(); }

  T *operator->() { return _object; }

  const T *operator->() const { return _object; }

  T &operator*() { return *_object; }

  const T &operator*() const { return *_object; }

  T *raw() { return _object; }

  const T *raw() const { return _object; }

  template <Ref K> AutoPtr<K> cast() { return dynamic_cast<K *>(_object); }

  template <Ref K> const AutoPtr<K> cast() const {
    return dynamic_cast<const K *>(_object);
  }

  operator bool() { return _object != nullptr; }

  bool operator==(const AutoPtr<T> &ptr) const {
    return _object == ptr._object;
  }

  bool operator==(const T *ptr) const { return _object == ptr; }

  AutoPtr<T> &operator=(const AutoPtr<T> &another) {
    if (_object == another._object) {
      return *this;
    }
    dispose();
    _object = another._object;
    addRef();
    return *this;
  }

  AutoPtr<T> &operator=(T *another) {
    if (_object == another) {
      return *this;
    }
    dispose();
    _object = another;
    addRef();
    return *this;
  }
};
} // namespace aleph::core