#pragma once
#include "TString.hpp"
#include <format>
#include <source_location>
#include <stdexcept>

namespace aleph::core {
class ExceptionBase : public std::runtime_error {
private:
  static std::string format(const std::string &type, const std::string &message,
                            const std::source_location &loc) {
    std::string result = std::format("{}: {}\n  at {} ({}:{}:{})", type,
                                     message, loc.function_name(),
                                     loc.file_name(), loc.line(), loc.column());
    return result;
  }
  static std::string format(const std::string &type, const std::string &message,
                            const std::exception &caused,
                            const std::source_location &loc) {
    std::string result =
        std::format("{}: {}\n  at {} ({}:{}:{})\ncaused by: {}", type, message,
                    loc.function_name(), loc.file_name(), loc.line(),
                    loc.column(), caused.what());
    return result;
  }

public:
  ExceptionBase(const ExceptionBase &another) = delete;

  ExceptionBase(
      const std::string &type, const std::string &message,
      const std::source_location &loc = std::source_location::current())
      : std::runtime_error(format(type, message, loc)) {}

  ExceptionBase(
      const std::string &type, const std::string &message,
      const std::exception &caused,
      const std::source_location &loc = std::source_location::current())
      : std::runtime_error(format(type, message, caused, loc)) {}
};
template <TString type> class Exception : public ExceptionBase {
public:
  Exception(const std::string &message = "",
            const std::source_location &loc = std::source_location::current())
      : ExceptionBase(type.value, message, loc) {}

  Exception(const std::string &message, const std::exception &caused,
            const std::source_location &loc = std::source_location::current())
      : ExceptionBase(type.value, message, caused, loc) {}
};
using NullpointerException = Exception<"NullpointerException">;
using IOException = Exception<"IOException">;
} // namespace aleph::core