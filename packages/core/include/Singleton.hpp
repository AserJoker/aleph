#pragma once
#include "core/include/AutoPtr.hpp"
namespace aleph::core {
template <class T> class Singleton {
public:
  static auto get() {
    static AutoPtr<T> instance;
    if (instance == nullptr) {
      instance = new T();
    }
    return instance;
  }
};
} // namespace aleph::core