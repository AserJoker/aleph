#pragma once
#include "Auto.hpp"
#include <type_traits>
namespace aleph::core {
template <typename T> class Singleton {
private:
  static inline Auto<T> _instance = nullptr;

public:
  static constexpr Auto<T> getInstance()
    requires std::is_default_constructible_v<T>
  {
    if (_instance == nullptr) {
      _instance = new T();
    }
    return _instance;
  }
  template <typename K = T, typename... ARGS>
  static constexpr Auto<T> initialize(ARGS... args)
    requires std::is_constructible_v<K, ARGS...> &&
             std::is_convertible_v<K *, T *>
  {
    _instance = new K(args...);
    return _instance;
  }
  static constexpr Auto<T> getInstance() { return _instance; }
  static void clear() { _instance = nullptr; }
};
} // namespace aleph::core
