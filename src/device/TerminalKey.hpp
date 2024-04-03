#pragma once
#include <string>
#include <vector>
struct TerminalKey {
  std::wstring name;
  std::wstring rawname;
  std::vector<wint_t> data;
  bool shift;
  bool ctrl;
  bool alt;
  bool visible;
};