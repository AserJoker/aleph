
#include <clocale>
#ifdef __linux__
#include "core/include/Size.hpp"
#include "system/include/ButtonEvent.hpp"
#include "system/include/InputEvent.hpp"
#include "system/include/Key.hpp"
#include "system/include/ResizeEvent.hpp"
#include "system/include/Terminal.hpp"
#include "system/include/WheelEvent.hpp"
#include <csignal>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

using namespace aleph;
using namespace aleph::system;
static termios g_origin = {};

Terminal::Terminal() {}

static winsize g_terminalSize = {};

void onResize(int) { ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_terminalSize); }

void Terminal::setup() {
  setlocale(LC_ALL, "");
  signal(SIGWINCH, &onResize);
  tcgetattr(STDIN_FILENO, &g_origin);
  auto current = g_origin;
  current.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  current.c_iflag &= ~(IXON | ICRNL | BRKINT | ISTRIP | INPCK);
  current.c_oflag &= ~(OPOST);
  current.c_cflag |= CS8;
  current.c_cc[VMIN] = 0;
  current.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &current);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_terminalSize);
}

void Terminal::cleanup() {
  while (!feof(stdin)) {
    fgetc(stdin);
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &g_origin);
}

int64_t Terminal::readInput() {
  if (!_codes.empty()) {
    auto ch = *_codes.begin();
    _codes.erase(_codes.begin());
    return ch;
  }
  fflush(STDIN_FILENO);
  uint8_t ch = 0;
  if (!read(STDIN_FILENO, &ch, 1)) {
    return Key::ERR;
  }
  return ch;
}
void Terminal::parseEvent() {
  auto ch = readInput();
  if (ch == Key::ERR) {
    return;
  }
  if (ch == 0) {
    emit(InputEvent{{Key::SPACE | Key::FLAG_CTRL}});
    return;
  }
  if (ch == 0x1f) {
    emit(InputEvent{{'/' | Key::FLAG_CTRL}});
    return;
  }
  if (ch == Key::ESC) {
    std::vector<int64_t> codes;
    ch = readInput();
    if (ch == Key::ERR) {
      emit(InputEvent{{Key::ESC}});
    } else if (ch == '[') {
      ch = readInput();
      if (ch == Key::ERR) {
        emit(InputEvent{{'[' | Key::FLAG_META}});
      } else if (ch == 'A') {
        emit(InputEvent{{Key::UP}});
      } else if (ch == 'B') {
        emit(InputEvent{{Key::DOWN}});
      } else if (ch == 'C') {
        emit(InputEvent{{Key::RIGHT}});
      } else if (ch == 'D') {
        emit(InputEvent{{Key::LEFT}});
      } else if (ch == 'H') {
        emit(InputEvent{{Key::HOME}});
      } else if (ch == 'F') {
        emit(InputEvent{{Key::END}});
      } else if (ch == 'Z') {
        emit(InputEvent{{Key::TAB | Key::FLAG_SHIFT}});
      } else if (ch == '<') {
        uint32_t btn = 0;
        int32_t x = 0;
        int32_t y = 0;
        std::vector<char> codes = {'<'};
        codes.push_back(ch);
        ch = readInput();
        if (ch >= '0' && ch <= '9') {
          while (ch >= '0' && ch <= '9') {
            codes.push_back((char)ch);
            btn = btn * 10 + (ch - '0');
            ch = readInput();
          }
          if (ch == ';') {
            codes.push_back(ch);
            ch = readInput();
            if (ch >= '0' && ch <= '9') {
              while (ch >= '0' && ch <= '9') {
                codes.push_back((char)ch);
                x = x * 10 + (ch - '0');
                ch = readInput();
              }
              if (ch == ';') {
                codes.push_back(ch);
                ch = readInput();
                if (ch >= '0' && ch <= '9') {
                  while (ch >= '0' && ch <= '9') {
                    codes.push_back((char)ch);
                    y = y * 10 + (ch - '0');
                    ch = readInput();
                  }
                  if (ch == 'm' || ch == 'M') {
                    _mouse.x = x;
                    _mouse.y = y;
                    if (btn >= 0 && btn <= 2) {
                      emit(ButtonEvent{btn, ch == 'M'});
                    } else if (btn >= 4 && btn <= 6) {
                      emit(ButtonEvent{btn - 4, ch == 'M', true});
                    } else if (btn >= 8 && btn <= 10) {
                      emit(ButtonEvent{btn - 8, ch == 'M', false, false, true});
                    } else if (btn >= 16 && btn <= 18) {
                      emit(
                          ButtonEvent{btn - 16, ch == 'M', false, true, false});
                    } else if (btn == 64 || btn == 68 || btn == 80 ||
                               btn == 75) {
                      emit(WheelEvent{true});
                    } else if (btn == 65 || btn == 69 || btn == 81 ||
                               btn == 76) {
                      emit(WheelEvent{false});
                    }
                    return;
                  }
                }
              }
            }
          }
        }
        if (ch != Key::ERR) {
          codes.push_back(ch);
        }
        for (auto &c : codes) {
          _codes.push_back(c);
        }
        emit(InputEvent{{'[' | Key::FLAG_META}});
      } else {
        std::vector<char> codes;
        std::string param1;
        while (ch >= '0' && ch <= '9') {
          codes.push_back((char)ch);
          param1.push_back((char)ch);
          ch = readInput();
        }
        if (!param1.empty()) {
          if (ch == '~') {
            if (param1 == "2") {
              emit(InputEvent{{Key::INSERT}});
              return;
            } else if (param1 == "3") {
              emit(InputEvent{{Key::DEL}});
              return;
            } else if (param1 == "5") {
              emit(InputEvent{{Key::PAGE_UP}});
              return;
            } else if (param1 == "6") {
              emit(InputEvent{{Key::PAGE_DOWN}});
              return;
            } else if (param1 == "15") {
              emit(InputEvent{{Key::F(5)}});
              return;
            } else if (param1 == "17") {
              emit(InputEvent{{Key::F(6)}});
              return;
            } else if (param1 == "18") {
              emit(InputEvent{{Key::F(7)}});
              return;
            } else if (param1 == "19") {
              emit(InputEvent{{Key::F(8)}});
              return;
            } else if (param1 == "20") {
              emit(InputEvent{{Key::F(9)}});
              return;
            } else if (param1 == "21") {
              emit(InputEvent{{Key::F(10)}});
              return;
            } else if (param1 == "23") {
              emit(InputEvent{{Key::F(11)}});
              return;
            } else if (param1 == "24") {
              emit(InputEvent{{Key::F(12)}});
              return;
            } else {
              codes.push_back(ch);
            }
          } else if (ch == ';') {
            codes.push_back(ch);
            ch = readInput();
            std::string param2;
            while (ch >= '0' && ch <= '9') {
              codes.push_back(ch);
              param2.push_back((char)ch);
              ch = readInput();
            }
            if (!param2.empty()) {
              if (param1 == "1") {
                int64_t flag = 0;
                if (param2 == "2") {
                  flag = Key::FLAG_SHIFT;
                } else if (param2 == "3") {
                  flag = Key::FLAG_META;
                } else if (param2 == "5") {
                  flag = Key::FLAG_CTRL;
                }
                if (flag) {
                  if (ch == 'A') {
                    emit(InputEvent{{Key::UP | flag}});
                    return;
                  }
                  if (ch == 'B') {
                    emit(InputEvent{{Key::DOWN | flag}});
                    return;
                  }
                  if (ch == 'C') {
                    emit(InputEvent{{Key::RIGHT | flag}});
                    return;
                  }
                  if (ch == 'D') {
                    emit(InputEvent{{Key::LEFT | flag}});
                    return;
                  }
                  if (ch == 'F') {
                    emit(InputEvent{{Key::END | flag}});
                    return;
                  }
                  if (ch == 'H') {
                    emit(InputEvent{{Key::HOME | flag}});
                    return;
                  }
                  if (ch == 'P') {
                    emit(InputEvent{{Key::F(1) | flag}});
                    return;
                  }
                  if (ch == 'Q') {
                    emit(InputEvent{{Key::F(2) | flag}});
                    return;
                  }
                  if (ch == 'R') {
                    emit(InputEvent{{Key::F(3) | flag}});
                    return;
                  }
                  if (ch == 'S') {
                    emit(InputEvent{{Key::F(4) | flag}});
                    return;
                  }
                }
              } else if (ch == '~') {
                int64_t flag = 0;
                if (param2 == "2") {
                  flag = Key::FLAG_SHIFT;
                } else if (param2 == "3") {
                  flag = Key::FLAG_META;
                } else if (param2 == "5") {
                  flag = Key::FLAG_CTRL;
                }
                if (flag) {
                  if (param1 == "2") {
                    emit(InputEvent{{Key::INSERT | flag}});
                    return;
                  }
                  if (param1 == "3") {
                    emit(InputEvent{{Key::DEL | flag}});
                    return;
                  }
                  if (param1 == "5") {
                    emit(InputEvent{{Key::PAGE_UP | flag}});
                    return;
                  }
                  if (param1 == "6") {
                    emit(InputEvent{{Key::PAGE_DOWN | flag}});
                    return;
                  }
                  if (param1 == "15") {
                    emit(InputEvent{{Key::F(5) | flag}});
                    return;
                  }
                  if (param1 == "17") {
                    emit(InputEvent{{Key::F(6) | flag}});
                    return;
                  }
                  if (param1 == "18") {
                    emit(InputEvent{{Key::F(7) | flag}});
                    return;
                  }
                  if (param1 == "19") {
                    emit(InputEvent{{Key::F(8) | flag}});
                    return;
                  }
                  if (param1 == "20") {
                    emit(InputEvent{{Key::F(9) | flag}});
                    return;
                  }
                  if (param1 == "21") {
                    emit(InputEvent{{Key::F(10) | flag}});
                    return;
                  }
                  if (param1 == "23") {
                    emit(InputEvent{{Key::F(11) | flag}});
                    return;
                  }
                  if (param1 == "24") {
                    emit(InputEvent{{Key::F(12) | flag}});
                    return;
                  }
                }
              }
            }
          }
        }
        if (ch != Key::ERR) {
          codes.push_back(ch);
        }
        for (auto &c : codes) {
          _codes.push_back(c);
        }
        emit(InputEvent{{'[' | Key::FLAG_META}});
      }
    } else if (ch == 'O') {
      ch = readInput();
      if (ch == Key::ERR) {
        emit(InputEvent{{'O' | Key::FLAG_META}});
      } else if (ch >= 'P' && ch <= 'S') {
        emit(InputEvent{{Key::F(ch - 'P' + 1)}});
      } else {
        _codes.push_back(ch);
        emit(InputEvent{{'O' | Key::FLAG_META}});
      }
    } else {
      emit(InputEvent{{ch | Key::FLAG_META}});
    }
    return;
  }
  std::vector<int64_t> codes;
  codes.push_back(ch);
  if (((uint8_t)ch) >> 7 & 1) {
    for (int8_t index = 0; index < 6; index++) {
      if (((((uint8_t)ch) >> ((6 - index)) & 1) & 0xf)) {
        auto next = readInput();
        codes.push_back(next);
      } else {
        break;
      }
    }
  }
  emit(InputEvent{codes});
}

void Terminal::pollEvent() {
  if (_size.height != g_terminalSize.ws_row ||
      _size.width != g_terminalSize.ws_col) {
    _size.height = g_terminalSize.ws_row;
    _size.width = g_terminalSize.ws_col;
    emit<ResizeEvent>(_size);
  }
  parseEvent();
}

void Terminal::flush() { fflush(stdout); }

void Terminal::move(int x, int y) { fprintf(stderr, "\033[%d;%dH", y, x); }

void Terminal::clear() { write(STDOUT_FILENO, "\033[2J", 4); }

void Terminal::setCursor(bool visible) {
  fprintf(stderr, "\033[?25%c", visible ? 'h' : 'l');
}

void Terminal::setMouse(bool enable) {
  fprintf(stderr, "\033[?1003;1006%c", enable ? 'h' : 'l');
}
void Terminal::saveCursor() { fprintf(stderr, "\033[s"); }

void Terminal::loadCursor() { fprintf(stderr, "\033[u"); }

const core::Size &Terminal::getSize() const { return _size; }

const core::Point &Terminal::getMousePosition() const { return _mouse; }

void Terminal::setNormal() { fprintf(stderr, "\033[0m"); }

void Terminal::setBold(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 1 : 22);
}

void Terminal::setHalfBright(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 2 : 22);
}

void Terminal::setItalic(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 3 : 23);
}

void Terminal::setUnderline(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 4 : 24);
}

void Terminal::setBlink(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 5 : 25);
}

void Terminal::setReverseVideo(bool enable) {
  fprintf(stderr, "\033[%dm", enable ? 7 : 27);
}

void Terminal::setColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  if (bright) {
    c += 60;
  }
  fprintf(stderr, "\033[%dm", c);
};

void Terminal::setBackgroundColor(const TERMINAL_COLOR_16 &color, bool bright) {
  char c = (char)color;
  c += 10;
  if (bright) {
    c += 60;
  }
  fprintf(stderr, "\033[%dm", c);
};

void Terminal::setColor(uint8_t color) {
  fprintf(stderr, "\033[38;5;%dm", color);
}

void Terminal::setBackgroundColor(uint8_t color) {
  fprintf(stderr, "\033[48;5;%dm", color);
}
void Terminal::setPalette(uint8_t idx, const core::Color &color) {
  fprintf(stderr, "\033]4;%d;rgb:%x/%x/%x\007", idx, color.r, color.g, color.b);
}

void Terminal::setPalette(uint8_t idx, uint32_t color) {
  setPalette(idx, core::Color{color});
}

void Terminal::setColor(const core::Color &color) {
  fprintf(stderr, "\033[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Terminal::setBackgroundColor(const core::Color &color) {
  fprintf(stderr, "\033[48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void Terminal::print(const std::string &str) {
  write(STDOUT_FILENO, str.c_str(), str.length());
}

std::string Terminal::compile(const Attr &attr) {
  std::vector<std::string> opts;
  if (attr.flag & Attr::BOLD) {
    opts.push_back("1");
  }
  if (attr.flag & Attr::HALF_BRIGHT) {
    opts.push_back("2");
  }
  if (attr.flag & Attr::ITALIC) {
    opts.push_back("3");
  }
  if (attr.flag & Attr::UNDERLINE) {
    opts.push_back("4");
  }
  if (attr.flag & Attr::BLINK) {
    opts.push_back("5");
  }
  if (attr.flag & Attr::COLOR) {
    opts.push_back("38;2");
    opts.push_back(std::to_string(attr.color.r));
    opts.push_back(std::to_string(attr.color.g));
    opts.push_back(std::to_string(attr.color.b));
  }
  if (attr.flag & Attr::BACKGROUND) {
    opts.push_back("48;2");
    opts.push_back(std::to_string(attr.background.r));
    opts.push_back(std::to_string(attr.background.g));
    opts.push_back(std::to_string(attr.background.b));
  }
  std::string result = "\033[";
  for (size_t index = 0; index < opts.size(); index++) {
    result += opts[index];
    if (index != opts.size() - 1) {
      result += ";";
    }
  }
  result += "m";
  return result;
}
#endif