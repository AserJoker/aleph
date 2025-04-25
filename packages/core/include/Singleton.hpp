#pragma once
#include "core/include/AutoPtr.hpp"
namespace aleph::core {
template <class T> class Singleton : public AutoPtr<T> {
public:
  static auto get() {
    static AutoPtr<T> instance;
    if (instance == nullptr) {
      instance = new T();
    }
    return instance;
  }
  
  Singleton() : AutoPtr<T>(get()) {}
};
} // namespace aleph::core