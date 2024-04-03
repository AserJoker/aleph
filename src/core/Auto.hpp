#pragma once
#include "Object.hpp"
#include <type_traits>
namespace aleph::core {
template <class T>
  requires std::is_convertible_v<T *, Object *>
class Auto {
private:
  Object *object;

public:
  Auto(T *obj = nullptr) : object(obj) {
    if (object) {
      object->addRef();
    }
  }
  Auto(const Auto<T> &another) : object(another.object) {
    if (object) {
      object->addRef();
    }
  }
  template <class K>
    requires std::is_convertible_v<K *, T *>
  Auto(const Auto<K> &another) : object(nullptr) {
    object = const_cast<K *>(another.get());
    if (object) {
      object->addRef();
    }
  }
  ~Auto() {
    if (object) {
      if (!object->dispose()) {
        delete object;
      }
    }
    object = nullptr;
  }
  Auto<T> &operator=(const Auto<T> &another) {
    if (object != another.object) {
      if (object) {
        object->dispose();
      }
      object = another.object;
      if (object) {
        object->addRef();
      }
    }
    return *this;
  }
  template <class K>
    requires std::is_convertible_v<K *, T *>
  Auto<T> &operator=(const Auto<K> &another) {
    if (object != another.get()) {
      if (object) {
        object->dispose();
      }
      object = another.object;
      if (object) {
        object->addRef();
      }
    }
    return *this;
  }
  Auto<T> &operator=(T *another) {
    if (object != another) {
      if (object) {
        object->dispose();
      }
      object = another;
      if (object) {
        object->addRef();
      }
    }
    return *this;
  }
  template <class K>
    requires std::is_convertible_v<K *, T *>
  Auto<T> &operator=(K *another) {
    if (object != another) {
      if (object) {
        object->dispose();
      }
      object = another;
      if (object) {
        object->addRef();
      }
    }
    return *this;
  }
  T *operator->() { return dynamic_cast<T *>(object); }
  T &operator*() { return *dynamic_cast<T *>(object); }

  const T *operator->() const { return dynamic_cast<const T *>(object); }
  const T &operator*() const { return *dynamic_cast<const T *>(object); }

  bool operator==(const Auto<T> &another) const {
    return object == another.object;
  }
  bool operator!=(const Auto<T> &another) const {
    return object != another.object;
  }
  bool operator==(const T *another) const { return object == another; }
  bool operator!=(const T *another) const { return object != another; }
  template <class K> Auto<K> cast() {
    return Auto<K>(dynamic_cast<K *>(object));
  }
  T *get() { return dynamic_cast<T *>(object); }
  const T *get() const { return dynamic_cast<const T *>(object); }
  static Auto<T> create(auto... args) { return new T(args...); }
};
} // namespace aleph::core
