#include "system/include/Terminal.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "system/include/ButtonPressEvent.hpp"
#include "system/include/KeyboardEvent.hpp"
#include "system/include/WheelEvent.hpp"
#include <cstdlib>
#include <stdexcept>
#ifdef PDCURSES
#define PDC_WIDE
#include <curses.h>
#define set_escdelay(val)
#define add_wchar(attr, ch)                                                    \
  do {                                                                         \
    attron(attr);                                                              \
    addch(ch);                                                                 \
    attroff(attr);                                                             \
  } while (0)
#define getmouse nc_getmouse
#define K_ESCAPE 0x1b
#define K_TAB 0x9
#define K_DEL 0x14a
#define K_INSERT 0x14b
#define K_HOME 0x106
#define K_END 0x166
#define K_PAGE_UP 0x153
#define K_PAGE_DOWN 0x152
#define K_ENTER 0xd
#define K_DOWN 0x102
#define K_UP 0x103
#define K_LEFT 0x104
#define K_RIGHT 0x105
#define K_BACKSPACE 0x8
#define K_F1 0x109
#define K_F2 0x10a
#define K_F3 0x10b
#define K_F4 0x10c
#define K_F5 0x10d
#define K_F6 0x10e
#define K_F7 0x10f
#define K_F8 0x110
#define K_F9 0x111
#define K_F10 0x112
#define K_F11 0x113
#define K_F12 0x114
#define K_SHIFT_TAB 0x15f
#define K_SHIFT_DEL 0x17d
#define K_SHIFT_INSERT 0x185
#define K_SHIFT_HOME 0x184
#define K_SHIFT_END 0x180
#define K_SHIFT_PAGE_UP 0x18c
#define K_SHIFT_PAGE_DOWN 0x18a
#define K_SHIFT_DOWN 0x224
#define K_SHIFT_UP 0x223
#define K_SHIFT_LEFT 0x187
#define K_SHIFT_RIGHT 0x190
#define K_SHIFT_F1 0x115
#define K_SHIFT_F2 0x116
#define K_SHIFT_F3 0x117
#define K_SHIFT_F4 0x118
#define K_SHIFT_F5 0x119
#define K_SHIFT_F6 0x11a
#define K_SHIFT_F7 0x11b
#define K_SHIFT_F8 0x11c
#define K_SHIFT_F9 0x11d
#define K_SHIFT_F10 0x11e
#define K_SHIFT_F11 0x11f
#define K_SHIFT_F12 0x120
#else
#ifdef NCURSES
#include <cstdio>
#include <ncursesw/ncurses.h>

#define add_wchar(attr, ch)                                                    \
  do {                                                                         \
    cchar_t cc = {attr, {ch, 0}};                                              \
    add_wch(&cc);                                                              \
  } while (0)
#endif
#endif

using namespace aleph;
using namespace aleph::system;

Terminal::Terminal() : core::Object() {
  initscr();
  scrollok(stdscr, FALSE);
  if (!has_colors()) {
    throw std::runtime_error("current terminal is not support color\n");
  }
  start_color();
  noecho();
  raw();
  keypad(stdscr, TRUE);
  curs_set(0);
  set_escdelay(0);
  nodelay(stdscr, TRUE);
  mouseinterval(0);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
#ifdef NCURSES
  ::printf("\033[?1003h\n");
#endif
}

Terminal::~Terminal() { endwin(); }

bool Terminal::processResize(int32_t input) {
  if (input == KEY_RESIZE) {
    emit(core::EventBase{"resize"});
    return true;
  }
  return false;
}

bool Terminal::processMouse(int32_t input) {
  MEVENT mouse;
  if (getmouse(&mouse) == OK) {
    _mouse.x = mouse.x;
    _mouse.y = mouse.y;
  }
  if (input == KEY_MOUSE) {
    if (mouse.bstate & BUTTON4_PRESSED) {
      emit(WheelEvent{true});
    }
    if (mouse.bstate & BUTTON5_PRESSED) {
      emit(WheelEvent{false});
    }
    if (mouse.bstate & BUTTON1_PRESSED) {
      emit(ButtonPressEvent{
          1,
          (mouse.bstate & BUTTON_SHIFT) != 0,
          (mouse.bstate & BUTTON_ALT) != 0,
          (mouse.bstate & BUTTON_CONTROL) != 0,
      });
    }
    if (mouse.bstate & BUTTON2_PRESSED) {
      emit(ButtonPressEvent{
          2,
          (mouse.bstate & BUTTON_SHIFT) != 0,
          (mouse.bstate & BUTTON_ALT) != 0,
          (mouse.bstate & BUTTON_CONTROL) != 0,
      });
    }
    if (mouse.bstate & BUTTON3_PRESSED) {
      emit(ButtonPressEvent{
          3,
          (mouse.bstate & BUTTON_SHIFT) != 0,
          (mouse.bstate & BUTTON_ALT) != 0,
          (mouse.bstate & BUTTON_CONTROL) != 0,
      });
    }
    return true;
  }
  return false;
}

bool Terminal::processKeyboard(int32_t input) {
  if (input == ERR) {
    return false;
  }
  switch (input) {
  case K_ESCAPE:
    emit(KeyboardEvent{L"escape"});
    break;
  case K_TAB:
    emit(KeyboardEvent{L"tab"});
    break;
  case K_ENTER:
    emit(KeyboardEvent{L"enter"});
    break;
  case K_BACKSPACE:
    emit(KeyboardEvent{L"backspace"});
    break;
  case K_DEL:
    emit(KeyboardEvent{L"delete"});
    break;
  case K_INSERT:
    emit(KeyboardEvent{L"insert"});
    break;
  case K_HOME:
    emit(KeyboardEvent{L"home"});
    break;
  case K_END:
    emit(KeyboardEvent{L"end"});
    break;
  case K_PAGE_UP:
    emit(KeyboardEvent{L"pageup"});
    break;
  case K_PAGE_DOWN:
    emit(KeyboardEvent{L"pagedown"});
    break;
  case K_LEFT:
    emit(KeyboardEvent{L"left"});
    break;
  case K_RIGHT:
    emit(KeyboardEvent{L"right"});
    break;
  case K_UP:
    emit(KeyboardEvent{L"up"});
    break;
  case K_DOWN:
    emit(KeyboardEvent{L"down"});
    break;
  case K_F1:
    emit(KeyboardEvent{L"f1"});
    break;
  case K_F2:
    emit(KeyboardEvent{L"f2"});
    break;
  case K_F3:
    emit(KeyboardEvent{L"f3"});
    break;
  case K_F4:
    emit(KeyboardEvent{L"f4"});
    break;
  case K_F5:
    emit(KeyboardEvent{L"f5"});
    break;
  case K_F6:
    emit(KeyboardEvent{L"f6"});
    break;
  case K_F7:
    emit(KeyboardEvent{L"f7"});
    break;
  case K_F8:
    emit(KeyboardEvent{L"f8"});
    break;
  case K_F9:
    emit(KeyboardEvent{L"f9"});
    break;
  case K_F10:
    emit(KeyboardEvent{L"f10"});
    break;
  case K_F11:
    emit(KeyboardEvent{L"f11"});
    break;
  case K_F12:
    emit(KeyboardEvent{L"f12"});
    break;
  case K_SHIFT_TAB:
    emit(KeyboardEvent{L"tab", true});
    break;
  case K_SHIFT_DEL:
    emit(KeyboardEvent{L"delete", true});
    break;
  case K_SHIFT_INSERT:
    emit(KeyboardEvent{L"insert", true});
    break;
  case K_SHIFT_HOME:
    emit(KeyboardEvent{L"home", true});
    break;
  case K_SHIFT_END:
    emit(KeyboardEvent{L"end", true});
    break;
  case K_SHIFT_PAGE_UP:
    emit(KeyboardEvent{L"pageup", true});
    break;
  case K_SHIFT_PAGE_DOWN:
    emit(KeyboardEvent{L"pagedown", true});
    break;
  case K_SHIFT_LEFT:
    emit(KeyboardEvent{L"left", true});
    break;
  case K_SHIFT_RIGHT:
    emit(KeyboardEvent{L"right", true});
    break;
  case K_SHIFT_UP:
    emit(KeyboardEvent{L"up", true});
    break;
  case K_SHIFT_DOWN:
    emit(KeyboardEvent{L"down", true});
    break;
  case K_SHIFT_F1:
    emit(KeyboardEvent{L"f1", true});
    break;
  case K_SHIFT_F2:
    emit(KeyboardEvent{L"f2", true});
    break;
  case K_SHIFT_F3:
    emit(KeyboardEvent{L"f3", true});
    break;
  case K_SHIFT_F4:
    emit(KeyboardEvent{L"f4", true});
    break;
  case K_SHIFT_F5:
    emit(KeyboardEvent{L"f5", true});
    break;
  case K_SHIFT_F6:
    emit(KeyboardEvent{L"f6", true});
    break;
  case K_SHIFT_F7:
    emit(KeyboardEvent{L"f7", true});
    break;
  case K_SHIFT_F8:
    emit(KeyboardEvent{L"f8", true});
    break;
  case K_SHIFT_F9:
    emit(KeyboardEvent{L"f9", true});
    break;
  case K_SHIFT_F10:
    emit(KeyboardEvent{L"f10", true});
    break;
  case K_SHIFT_F11:
    emit(KeyboardEvent{L"f11", true});
    break;
  case K_SHIFT_F12:
    emit(KeyboardEvent{L"f12", true});
    break;
  default: {
    if (input >= 0x20 && input <= 0x7e || input > 0xff) {
      wchar_t buf[2] = {(wchar_t)input, 0};
      emit(KeyboardEvent{buf});
    } else {
      wchar_t buf[16];
      swprintf(buf, L"0x%x", input);
      emit(KeyboardEvent{buf});
    }
    break;
  }
  }
  return true;
}

void Terminal::pollEvent() {
  auto ch = getch();
  if (processMouse(ch)) {
    return;
  }
  if (processResize(ch)) {
    return;
  }
  if (processKeyboard(ch)) {
    return;
  }
}

void Terminal::print(int x, int y, const wchar_t *msg) {
  for (int index = 0; msg[index] != 0; index++) {
    cchar_t ch = msg[index];
    mvaddch(y, x + index, ch);
  }
}

void Terminal::setColor(uint32_t fg, uint32_t bg) {
  if (!_palette.contains(fg)) {
    auto b = (uint8_t)(fg & 0xff);
    auto g = (uint8_t)((fg & 0xffff) >> 8);
    auto r = (uint8_t)((fg & 0xffffff) >> 16);
    auto idx = (short)_palette.size() + 17;
    _palette[fg] = idx;
    init_color(idx, (short)(r / 255.0f * 1000.0f),
               (short)(g / 255.0f * 1000.0f), (short)(b / 255.0f * 1000.0f));
  }
  if (!_palette.contains(bg)) {
    auto b = (uint8_t)(bg & 0xff);
    auto g = (uint8_t)((bg & 0xffff) >> 8);
    auto r = (uint8_t)((bg & 0xffffff) >> 16);
    auto idx = (short)_palette.size() + 17;
    _palette[bg] = idx;
    init_color(idx, (short)(r / 255.0f * 1000.0f),
               (short)(g / 255.0f * 1000.0f), (short)(b / 255.0f * 1000.0f));
  }
  uint32_t fghandle = _palette.at(fg);
  uint32_t bghandle = _palette.at(bg);
  uint32_t color = (fghandle << 16) | bghandle;
  if (!_indices.contains(color)) {
    auto idx = (short)_indices.size() + 10;
    init_pair(idx, fghandle, bghandle);
    _indices[color] = idx;
  }
  attron(COLOR_PAIR(_indices.at(color)));
}

core::Size Terminal::getTerminalSize() const {
  core::Size size;
  getmaxyx(stdscr, size.height, size.width);
  return size;
}

const core::Point &Terminal::getMousePosition() const { return _mouse; }