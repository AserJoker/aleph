#pragma once
#include "Exception.hpp"
#include "core/include/ObjectBase.hpp"
#include "core/include/Singleton.hpp"
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace aleph::core {
class LoggerWriter : public ObjectBase {
public:
  virtual void write(const std::string &message) = 0;
};
template <TString name> class FileWriter : public LoggerWriter {
private:
  std::ofstream _output;

public:
  FileWriter() {
    using namespace std::filesystem;
    using namespace std::chrono;
    if (exists(name.value)) {
      auto now = floor<seconds>(system_clock::now());
      auto time = system_clock::to_time_t(now);
      std::stringstream ss;
      ss << std::put_time(std::localtime(&time), "%Y-%m-%d-%H-%M-%S");
      rename(name.value, std::format("{}.{}", name.value, ss.str()));
    }
    _output.open(name.value, std::ios::app | std::ios::out);
    if (!_output.is_open()) {
      throw IOException(std::format("Failed to open log file: {}", name.value));
    }
  }

  ~FileWriter() override { _output.close(); }

  void write(const std::string &message) override {
    _output << message << std::endl;
  }
};

class ConsoleWriter : public LoggerWriter {
public:
  void write(const std::string &message) override {
    std::cout << message << std::endl;
  }
};

enum class LoggerLevel { DEBUG = 0, INFO, LOG, WARN, ERROR };

template <typename Writer = ConsoleWriter, LoggerLevel mask = LoggerLevel::LOG>
class Logger : public ObjectBase {
private:
  static std::string format(const LoggerLevel &level,
                            const std::string &message) {
    std::string levelstr;
    switch (level) {
    case LoggerLevel::DEBUG:
      levelstr = "DEBUG";
      break;
    case LoggerLevel::INFO:
      levelstr = "INFO";
      break;
    case LoggerLevel::LOG:
      levelstr = "LOG";
      break;
    case LoggerLevel::WARN:
      levelstr = "WARN";
      break;
    case LoggerLevel::ERROR:
      levelstr = "ERROR";
      break;
    }
    using namespace std::chrono;
    auto now = floor<seconds>(system_clock::now());
    auto time = system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%F %T");
    return std::format("{} [{}]: {}", ss.str(), levelstr, message);
  }

public:
  template <typename... Args>
  static inline void debug(std::format_string<Args...> fmt, Args &&...args) {
    if (mask < LoggerLevel::DEBUG) {
      return;
    }
    core::Singleton<Writer>::get()->write(
        format(LoggerLevel::DEBUG,
               std::vformat(fmt.get(), std::make_format_args(args...))));
  }

  template <typename... Args>
  static inline void info(std::format_string<Args...> fmt, Args &&...args) {
    if (mask < LoggerLevel::INFO) {
      return;
    }
    core::Singleton<Writer>::get()->write(
        format(LoggerLevel::INFO,
               std::vformat(fmt.get(), std::make_format_args(args...))));
  }

  template <typename... Args>
  static inline void log(std::format_string<Args...> fmt, Args &&...args) {
    if (mask < LoggerLevel::LOG) {
      return;
    }
    core::Singleton<Writer>::get()->write(
        format(LoggerLevel::LOG,
               std::vformat(fmt.get(), std::make_format_args(args...))));
  }

  template <typename... Args>
  static inline void warn(std::format_string<Args...> fmt, Args &&...args) {
    if (mask < LoggerLevel::WARN) {
      return;
    }
    core::Singleton<Writer>::get()->write(
        format(LoggerLevel::WARN,
               std::vformat(fmt.get(), std::make_format_args(args...))));
  }

  template <typename... Args>
  static inline void error(std::format_string<Args...> fmt, Args &&...args) {
    if (mask < LoggerLevel::ERROR) {
      return;
    }
    core::Singleton<Writer>::get()->write(
        format(LoggerLevel::ERROR,
               std::vformat(fmt.get(), std::make_format_args(args...))));
  }
};
}; // namespace aleph::core