#include "system/include/Terminal.hpp"
#include <clocale>
#include <cstdio>
#include <cwchar>
#include <format>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace aleph;
int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  system::Terminal terminal;
  std::string input;
  terminal.setup();
  terminal.saveCursor();
  terminal.setMouse(true);
  terminal.setCursor(false);
  terminal.clear();
  terminal.present();
  while (true) {
    std::vector<int> codes;
    while (true) {
      int ch = terminal.readInput();
      if (ch == -1) {
        break;
      }
      codes.push_back(ch);
    }
    if (codes.size()) {
      input = "";
      if (codes.size() == 1 && codes[0] == 'q') {
        break;
      }
      if (codes.size() == 1 && codes[0] == 'r') {
        terminal.clear();
      } else {
        for (auto &ch : codes) {
          input += std::format("0x{:x}", ch);
          input += ",";
        }
        terminal.move(1, 2);
        printf("%s", input.c_str());
      }
      terminal.present();
    } else {
      using namespace std::chrono;
      std::this_thread::sleep_for(10ms);
    }
  }
  terminal.clear();
  terminal.present();
  terminal.setCursor(true);
  terminal.setMouse(false);
  terminal.move(1, 1);
  terminal.cleanup();
  return 0;
}
// #include <stdio.h>
// #include <sys/ioctl.h>
// #include <termios.h>
// #include <unistd.h>

// enum {
//   KEYSTATE_NONE = 1024,
//   KEYSTATE_ESCAPE,
//   KEYSTATE_CONTROL,
//   KEYSTATE_MOUSE_PROPS
// };

// enum {
//   KEYSTATE_MOUSE_PRESSED,
//   KEYSTATE_MOUSE_RELEASED,
//   KEYSTATE_MOUSE_GOT_STATE,
//   KEYSTATE_MOUSE_GOT_X,
//   KEYSTATE_MOUSE_GOT_Y
// };

// int mygetch(void) {
//   struct termios oldt, newt;
//   int ch;
//   tcgetattr(STDIN_FILENO, &oldt);
//   newt = oldt;
//   newt.c_lflag &= ~(ICANON | ECHO);
//   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//   ch = getchar();
//   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//   return ch;
// }

// int get_terminal_width() {
//   struct winsize w;
//   ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//   return w.ws_col;
// }

// int get_terminal_height() {
//   struct winsize w;
//   ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//   return w.ws_row;
// }

// int twidth, theight;
// int mouse_state, mouse_x, mouse_y;

// int main() {
//   twidth = get_terminal_width();
//   theight = get_terminal_height() - 5;

//   printf("\033[?1000h");
//   // printf("\033[?1006h");
//   // printf("\e]12;black\a");

//   printf("lines %d\n", get_terminal_height());
//   printf("columns \e[41m%d\e[0m\n", get_terminal_width());

//   char c;
//   int special = KEYSTATE_NONE;

//   while ((c = mygetch())) {
//     if (special == KEYSTATE_NONE && c == 27) {
//       special = KEYSTATE_ESCAPE;
//       continue;
//     }
//     if (special == KEYSTATE_ESCAPE && c == 91) {
//       special = KEYSTATE_CONTROL;
//       continue;
//     }
//     if (special == KEYSTATE_CONTROL)
//       switch (c) {
//       case 'A':
//         printf("up\n");
//         special = KEYSTATE_NONE;
//         continue;
//         break;
//       case 'B':
//         printf("down \n");
//         special = KEYSTATE_NONE;
//         continue;
//         break;
//       case 'D':
//         printf("right\n");
//         special = KEYSTATE_NONE;
//         continue;
//         break;
//       case 'C':
//         printf("left\n");
//         special = KEYSTATE_NONE;
//         continue;
//         break;
//       }

//     if (c == 'M') {
//       printf("mouse ");
//       special = KEYSTATE_MOUSE_PROPS;
//       continue;
//     }

//     if (special == KEYSTATE_MOUSE_PROPS) {
//       if (c == 32)
//         mouse_state = KEYSTATE_MOUSE_PRESSED;
//       if (c == 35)
//         mouse_state = KEYSTATE_MOUSE_RELEASED;
//       special = KEYSTATE_MOUSE_GOT_STATE;
//       continue;
//     }

//     if (special == KEYSTATE_MOUSE_GOT_STATE) {
//       mouse_x = c - 32;
//       if (mouse_x < 0)
//         mouse_x += 255;
//       special = KEYSTATE_MOUSE_GOT_X;
//       continue;
//     }

//     if (special == KEYSTATE_MOUSE_GOT_X) {
//       mouse_y = c - 32;
//       if (mouse_y < 0)
//         mouse_y += 255;
//       special = KEYSTATE_MOUSE_GOT_Y;
//     }

//     if (special == KEYSTATE_MOUSE_GOT_Y) {
//       special = KEYSTATE_NONE;
//       printf("Mouse: STATE=%d\tX=%d\tY=%d\n", mouse_state, mouse_x, mouse_y);
//       continue;
//     }

//     printf("Char is ");
//     if (c >= 'A')
//       printf("%c (", c);
//     printf("%d", c);
//     if (c >= 'A')
//       printf(")");
//     printf("\n");
//   }
//   printf("\n");

//   return 0;
// }