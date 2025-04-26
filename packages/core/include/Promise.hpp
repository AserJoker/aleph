#pragma once
#include "core/include/AutoPtr.hpp"
#include "core/include/Co.hpp"
#include "core/include/ObjectBase.hpp"
#include "core/include/Singleton.hpp"
#include <exception>
namespace aleph::core {
template <class T> class Promise : public ObjectBase {
public:
  enum class Status {
    PENDING,
    FULFILLED,
    REJECTED,
  };

private:
  T _value{};
  std::exception _exception{};
  Status _status = {Status::PENDING};

public:
  Promise() {}

  void resolve(const T &value) {
    if (_status == Status::PENDING) {
      _value = value;
      _status = Status::FULFILLED;
    }
  }

  void reject(const std::exception &exception) {
    if (_status == Status::PENDING) {
      _exception = exception;
      _status = Status::REJECTED;
    }
  }

  const Status &getStatus() const { return _status; }

  const T &wait() const {
    while (_status == Status::PENDING) {
      Singleton<Co>::get()->yield();
    }
    if (_status == Status::FULFILLED) {
      return _value;
    }
    throw _exception;
  }
};

template <> class Promise<void> : public ObjectBase {

public:
  enum class Status {
    PENDING,
    FULFILLED,
    REJECTED,
  };

private:
  std::exception _exception{};
  Status _status = {Status::PENDING};

public:
  Promise() {}

  void resolve() {
    if (_status == Status::PENDING) {
      _status = Status::FULFILLED;
    }
  }

  void reject(const std::exception &exception) {
    if (_status == Status::PENDING) {
      _exception = exception;
      _status = Status::REJECTED;
    }
  }

  const Status &getStatus() const { return _status; }

  void wait() {
    while (_status == Status::PENDING) {
      Singleton<Co>::get()->yield();
    }
    if (_status == Status::FULFILLED) {
      return;
    }
    throw _exception;
  }
};
} // namespace aleph::core