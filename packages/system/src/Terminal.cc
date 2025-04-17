#include "system/include/Terminal.hpp"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "system/include/ButtonPressEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/WheelEvent.hpp"
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

void Terminal::pollEvent() {
  auto ch = getch();
  if (processMouse(ch)) {
    return;
  }
  if (processResize(ch)) {
    return;
  }
  if (ch != ERR) {
    emit(InputEvent{ch});
  }
}

void Terminal::print(int x, int y, const char *msg) {
  mvprintw(y, x, "%s", msg);
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