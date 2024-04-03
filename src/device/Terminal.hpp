#pragma once
#include "../core/Object.hpp"
#include "../framework/Application.hpp"
#include "../framework/EventTick.hpp"
#include "Attribute.hpp"
#include "EventInput.hpp"
#include "Symbol.hpp"
#include "TerminalKey.hpp"
#include <curses.h>
#include <fmt/xchar.h>
#include <locale>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace aleph::device {
enum class TerminalMode { BASE, COLOR, COLOREX };
class Terminal : public core::Object {
public:
private:
  TerminalMode mode;
  static inline const std::vector<TerminalKey> keymap = {
      TerminalKey{.name = L"<esc>",
                  .rawname = L"<esc>",
                  .data = {27},
                  .shift = false,
                  .ctrl = false,
                  .alt = false},
      TerminalKey{
          .name = L"<resize>",
          .rawname = L"<resize>",
          .data = {410},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<tab>",
          .rawname = L"<tab>",
          .data = {9},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<enter>",
          .rawname = L"<enter>",
          .data = {10},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<backspace>",
          .rawname = L"<backspace>",
          .data = {127},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<left>",
          .rawname = L"<left>",
          .data = {27, 91, 68},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<right>",
          .rawname = L"<right>",
          .data = {27, 91, 67},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<down>",
          .rawname = L"<down>",
          .data = {27, 91, 66},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<up>",
          .rawname = L"<up>",
          .data = {27, 91, 65},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<end>",
          .rawname = L"<end>",
          .data = {27, 91, 70},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<home>",
          .rawname = L"<home>",
          .data = {27, 91, 72},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-tab>",
          .rawname = L"<tab>",
          .data = {27, 91, 90},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f5>",
          .rawname = L"<f5>",
          .data = {27, 91, 49, 53, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f5>",
          .rawname = L"<f5>",
          .data = {27, 91, 49, 53, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f5>",
          .rawname = L"<f5>",
          .data = {27, 91, 49, 53, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f5>",
          .rawname = L"<f5>",
          .data = {27, 91, 49, 53, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<f6>",
          .rawname = L"<f6>",
          .data = {27, 91, 49, 55, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f6>",
          .rawname = L"<f6>",
          .data = {27, 91, 49, 55, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f6>",
          .rawname = L"<f6>",
          .data = {27, 91, 49, 55, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f6>",
          .rawname = L"<f6>",
          .data = {27, 91, 49, 55, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-f6>",
          .rawname = L"<f6>",
          .data = {27, 91, 49, 55, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<f7>",
          .rawname = L"<f7>",
          .data = {27, 91, 49, 56, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f7>",
          .rawname = L"<f7>",
          .data = {27, 91, 49, 56, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f7>",
          .rawname = L"<f7>",
          .data = {27, 91, 49, 56, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f7>",
          .rawname = L"<f7>",
          .data = {27, 91, 49, 56, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f7>",
          .rawname = L"<f7>",
          .data = {27, 91, 49, 56, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<f8>",
          .rawname = L"<f8>",
          .data = {27, 91, 49, 57, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f8>",
          .rawname = L"<f8>",
          .data = {27, 91, 49, 57, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f8>",
          .rawname = L"<f8>",
          .data = {27, 91, 49, 57, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f8>",
          .rawname = L"<f8>",
          .data = {27, 91, 49, 57, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f8>",
          .rawname = L"<f8>",
          .data = {27, 91, 49, 57, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<s-left>",
          .rawname = L"<left>",
          .data = {27, 91, 49, 59, 50, 68},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-right>",
          .rawname = L"<right>",
          .data = {27, 91, 49, 59, 50, 67},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-down>",
          .rawname = L"<down>",
          .data = {27, 91, 49, 59, 50, 66},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-up>",
          .rawname = L"<up>",
          .data = {27, 91, 49, 59, 50, 65},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-end>",
          .rawname = L"<end>",
          .data = {27, 91, 49, 59, 50, 70},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-home>",
          .rawname = L"<home>",
          .data = {27, 91, 49, 59, 50, 72},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f1>",
          .rawname = L"<f1>",
          .data = {27, 91, 49, 59, 50, 80},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f2>",
          .rawname = L"<f2>",
          .data = {27, 91, 49, 59, 50, 81},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f3>",
          .rawname = L"<f3>",
          .data = {27, 91, 49, 59, 50, 82},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f4>",
          .rawname = L"<f4>",
          .data = {27, 91, 49, 59, 50, 83},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-left>",
          .rawname = L"<left>",
          .data = {27, 91, 49, 59, 51, 68},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-right>",
          .rawname = L"<right>",
          .data = {27, 91, 49, 59, 51, 67},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-down>",
          .rawname = L"<down>",
          .data = {27, 91, 49, 59, 51, 66},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-up>",
          .rawname = L"<up>",
          .data = {27, 91, 49, 59, 51, 65},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-end>",
          .rawname = L"<end>",
          .data = {27, 91, 49, 59, 51, 70},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-home>",
          .rawname = L"<home>",
          .data = {27, 91, 49, 59, 51, 72},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-f1>",
          .rawname = L"<f1>",
          .data = {27, 91, 49, 59, 51, 80},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-f2>",
          .rawname = L"<f2>",
          .data = {27, 91, 49, 59, 51, 81},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-f3>",
          .rawname = L"<f3>",
          .data = {27, 91, 49, 59, 51, 82},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<m-f4>",
          .rawname = L"<f4>",
          .data = {27, 91, 49, 59, 51, 83},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-left>",
          .rawname = L"<left>",
          .data = {27, 91, 49, 59, 53, 68},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-right>",
          .rawname = L"<right>",
          .data = {27, 91, 49, 59, 53, 67},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-down>",
          .rawname = L"<down>",
          .data = {27, 91, 49, 59, 53, 66},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-up>",
          .rawname = L"<up>",
          .data = {27, 91, 49, 59, 53, 65},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-end>",
          .rawname = L"<end>",
          .data = {27, 91, 49, 59, 53, 70},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-home>",
          .rawname = L"<home>",
          .data = {27, 91, 49, 59, 53, 72},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f1>",
          .rawname = L"<f1>",
          .data = {27, 91, 49, 59, 53, 80},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f2>",
          .rawname = L"<f2>",
          .data = {27, 91, 49, 59, 53, 81},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f3>",
          .rawname = L"<f3>",
          .data = {27, 91, 49, 59, 53, 82},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f4>",
          .rawname = L"<f4>",
          .data = {27, 91, 49, 59, 53, 83},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-left>",
          .rawname = L"<left>",
          .data = {27, 91, 49, 59, 55, 68},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-right>",
          .rawname = L"<right>",
          .data = {27, 91, 49, 59, 55, 67},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-down>",
          .rawname = L"<down>",
          .data = {27, 91, 49, 59, 55, 66},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-up>",
          .rawname = L"<up>",
          .data = {27, 91, 49, 59, 55, 65},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-home>",
          .rawname = L"<home>",
          .data = {27, 91, 49, 59, 55, 72},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-end>",
          .rawname = L"<end>",
          .data = {27, 91, 49, 59, 55, 70},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-f1>",
          .rawname = L"<f1>",
          .data = {27, 91, 49, 59, 55, 80},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-f2>",
          .rawname = L"<f2>",
          .data = {27, 91, 49, 59, 55, 81},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-f3>",
          .rawname = L"<f3>",
          .data = {27, 91, 49, 59, 55, 82},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-m-f4>",
          .rawname = L"<f4>",
          .data = {27, 91, 49, 59, 55, 83},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<s-f9>",
          .rawname = L"<f9>",
          .data = {27, 91, 50, 48, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f9>",
          .rawname = L"<f9>",
          .data = {27, 91, 50, 48, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f9>",
          .rawname = L"<f9>",
          .data = {27, 91, 50, 48, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f9>",
          .rawname = L"<f9>",
          .data = {27, 91, 50, 48, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<insert>",
          .rawname = L"<insert>",
          .data = {27, 91, 50, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-insert>",
          .rawname = L"<insert>",
          .data = {27, 91, 50, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-insert>",
          .rawname = L"<insert>",
          .data = {27, 91, 50, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-insert>",
          .rawname = L"<insert>",
          .data = {27, 91, 50, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f9>",
          .rawname = L"<f9>",
          .data = {27, 91, 50, 48, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f10>",
          .rawname = L"<f10>",
          .data = {27, 91, 50, 49, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f10>",
          .rawname = L"<f10>",
          .data = {27, 91, 50, 49, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f10>",
          .rawname = L"<f10>",
          .data = {27, 91, 50, 49, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f10>",
          .rawname = L"<f10>",
          .data = {27, 91, 50, 49, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f10>",
          .rawname = L"<f10>",
          .data = {27, 91, 50, 49, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<f11>",
          .rawname = L"<f11>",
          .data = {27, 91, 50, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f11>",
          .rawname = L"<f11>",
          .data = {27, 91, 50, 51, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f11>",
          .rawname = L"<f11>",
          .data = {27, 91, 50, 51, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f11>",
          .rawname = L"<f11>",
          .data = {27, 91, 50, 51, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f11>",
          .rawname = L"<f11>",
          .data = {27, 91, 50, 51, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<f12>",
          .rawname = L"<f12>",
          .data = {27, 91, 50, 52, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-f12>",
          .rawname = L"<f12>",
          .data = {27, 91, 50, 52, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-f12>",
          .rawname = L"<f12>",
          .data = {27, 91, 50, 52, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-f12>",
          .rawname = L"<f12>",
          .data = {27, 91, 50, 52, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m-f12>",
          .rawname = L"<f12>",
          .data = {27, 91, 50, 52, 59, 55, 126},
          .shift = false,
          .ctrl = true,
          .alt = true,
      },
      TerminalKey{
          .name = L"<delete>",
          .rawname = L"<delete>",
          .data = {27, 91, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-delete>",
          .rawname = L"<delete>",
          .data = {27, 91, 51, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-delete>",
          .rawname = L"<delete>",
          .data = {27, 91, 51, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-delete>",
          .rawname = L"<delete>",
          .data = {27, 91, 51, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<pgup>",
          .rawname = L"<pgup>",
          .data = {27, 91, 53, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-pgup>",
          .rawname = L"<pgup>",
          .data = {27, 91, 53, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-pgup>",
          .rawname = L"<pgup>",
          .data = {27, 91, 53, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-pgup>",
          .rawname = L"<pgup>",
          .data = {27, 91, 53, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<pgdn>",
          .rawname = L"<pgdn>",
          .data = {27, 91, 54, 126},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<s-pgdn>",
          .rawname = L"<pgdn>",
          .data = {27, 91, 54, 59, 50, 126},
          .shift = true,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<m-pgdn>",
          .rawname = L"<pgdn>",
          .data = {27, 91, 54, 59, 51, 126},
          .shift = false,
          .ctrl = false,
          .alt = true,
      },
      TerminalKey{
          .name = L"<c-pgdn>",
          .rawname = L"<pgdn>",
          .data = {27, 91, 54, 59, 53, 126},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f1>",
          .rawname = L"<f1>",
          .data = {27, 79, 80},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f2>",
          .rawname = L"<f2>",
          .data = {27, 79, 81},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f3>",
          .rawname = L"<f3>",
          .data = {27, 79, 82},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<f4>",
          .rawname = L"<f4>",
          .data = {27, 79, 83},
          .shift = false,
          .ctrl = false,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-a>",
          .rawname = L"a",
          .data = {1},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-b>",
          .rawname = L"b",
          .data = {2},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-c>",
          .rawname = L"c",
          .data = {3},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-d>",
          .rawname = L"d",
          .data = {4},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-e>",
          .rawname = L"e",
          .data = {5},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-f>",
          .rawname = L"f",
          .data = {6},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-g>",
          .rawname = L"g",
          .data = {7},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-h>",
          .rawname = L"h",
          .data = {8},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-i>",
          .rawname = L"i",
          .data = {9},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-j>",
          .rawname = L"j",
          .data = {10},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-k>",
          .rawname = L"k",
          .data = {11},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-l>",
          .rawname = L"l",
          .data = {12},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-m>",
          .rawname = L"m",
          .data = {13},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-n>",
          .rawname = L"n",
          .data = {14},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-o>",
          .rawname = L"o",
          .data = {15},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-p>",
          .rawname = L"p",
          .data = {16},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-q>",
          .rawname = L"q",
          .data = {17},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-r>",
          .rawname = L"r",
          .data = {18},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-s>",
          .rawname = L"s",
          .data = {19},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-t>",
          .rawname = L"t",
          .data = {20},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-u>",
          .rawname = L"u",
          .data = {21},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-v>",
          .rawname = L"v",
          .data = {22},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-w>",
          .rawname = L"w",
          .data = {23},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-x>",
          .rawname = L"x",
          .data = {24},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-y>",
          .rawname = L"y",
          .data = {25},
          .shift = false,
          .ctrl = true,
          .alt = false,
      },
      TerminalKey{
          .name = L"<c-z>",
          .rawname = L"z",
          .data = {26},
          .shift = false,
          .ctrl = true,
          .alt = false,
      }};

  void initVideo() {
    if (mode > TerminalMode::BASE) {
      if (has_colors()) {
        start_color();
        if (mode > TerminalMode::COLOR) {
          if (!can_change_color()) {
            mode = TerminalMode::COLOR;
          }
        }
      }
    }
  }
  void initInput() {
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    noqiflush();
    intrflush(stdscr, FALSE);
    notimeout(stdscr, TRUE);
    curs_set(0);
  }

private:
  struct Item {
    int32_t x;
    int32_t y;
    Symbol symbol;
    bool changed;
  };
  std::unordered_map<std::string, Item> buffers;

protected:
  virtual void onTick(const framework::EventTick &) {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    auto app = core::Singleton<framewrok::Application>::getInstance();
    std::vector<wint_t> keycodes;
    wint_t key;
    while (get_wch(&key) != ERR) {
      keycodes.push_back(key);
    }
    TerminalKey encodeKey;
    if (keycodes.size() > 0) {
      for (auto &key : keymap) {
        bool flag = true;
        for (auto i = 0; i < keycodes.size(); i++) {
          if (i >= key.data.size() || key.data[i] != keycodes[i]) {
            flag = false;
            break;
          }
        }
        if (flag) {
          encodeKey = key;
          break;
        }
      }
      if (encodeKey.name == L"") {
        std::wstring name;
        for (auto ch : keycodes) {
          name += (wchar_t)ch;
        }
        encodeKey = {.name = name,
                     .rawname = name,
                     .data = keycodes,
                     .shift = false,
                     .ctrl = false,
                     .alt = false,
                     .visible = true};
      } else {
        encodeKey.visible = false;
      }
      bus->emit(EventInput(encodeKey));
    }
  }

  virtual void onPreInitialize(const framework::EventPreInitialize &) {
    auto app = core::Singleton<framewrok::Application>::getInstance();
    std::locale::global(std::locale(""));
    initscr();
    intrflush(stdscr, FALSE);
    initVideo();
    initInput();
  }
  virtual void onUnInitialize(const framework::EventUnInitialize &) {
    endwin();
  }

public:
  Terminal(TerminalMode mode) : mode(mode){};
  void initialize() {
    auto bus = core::Singleton<core::EventBus>::getInstance();
    bus->on(this, &Terminal::onPreInitialize);
    bus->on(this, &Terminal::onUnInitialize);
    bus->on(this, &Terminal::onTick);
  }
  const TerminalMode &getMode() const { return mode; }

  void draw(int32_t x, int32_t y, wchar_t ch, const Attribute &attr) {
    // cchar_t cc = {attr.getIdentity(), {ch, 0}};
    // mvadd_wch(y, x, &cc);
    auto key = fmt::format("{},{}", x, y);
    if (buffers.contains(key)) {
      auto &item = buffers[key];
      if (item.symbol.ch != ch ||
          item.symbol.attr.getIdentity() != attr.getIdentity()) {
        item.symbol = {ch, attr};
        item.changed = true;
      }
    } else {
      buffers[key] = {x, y, {ch, attr}, true};
    }
  }
  void present() {
    using namespace std::chrono;
    bool isChanged = false;
    for (auto &[_, item] : buffers) {
      if (item.changed) {
        if (!isChanged) {
          ::clear();
          isChanged = true;
        }
        cchar_t cc = {item.symbol.attr.getIdentity(), {item.symbol.ch}};
        mvadd_wch(item.y, item.x, &cc);
        item.changed = false;
      }
    }
    if (isChanged) {
      wrefresh(stdscr);
      std::this_thread::sleep_for(40ms);
    }
  }
  void draw(int32_t x, int32_t y, const Symbol &symbol) {
    auto &[ch, attr] = symbol;
    draw(x, y, ch, attr);
  }
  void getTerminalSize(int32_t &width, int32_t &height) {
    width = getmaxx(stdscr);
    height = getmaxy(stdscr);
  }
  void clear() {
    int32_t w, h;
    getTerminalSize(w, h);
    for (auto x = 0; x < w; x++) {
      for (auto y = 0; y < h; y++) {
        draw(x, y, L' ');
      }
    }
  }
};
}; // namespace aleph::device
