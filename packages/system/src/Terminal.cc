#include "system/include/Terminal.h"
#include "core/include/EventBase.hpp"
#include "core/include/Object.hpp"
#include "system/include/InputEvent.h"
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
#else
#ifdef NCURSES
#include <ncursesw/ncurses.h>
#include <cstdio>
#define add_wchar(attr, ch)                                                    \
  do {                                                                         \
    cchar_t cc = {attr, {ch, 0}};                                              \
    add_wch(&cc);                                                              \
  } while (0)
#endif
#endif
#define EVENT_RESIZE 0x222
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
  if (has_mouse()) {
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
#ifdef NCURSES
    ::printf("\033[?1003h\n");
#endif
  }
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
  if (input == KEY_MOUSE) {
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
  emit(InputEvent{ch});
}

void Terminal::print(int x, int y, const char *msg) { mvprintw(y, x, msg); }

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