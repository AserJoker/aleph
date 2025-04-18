#include <clocale>
#include <conio.h>
#include <cstdio>
#include <windows.h>
int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOutput, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOutput, mode);
  printf("\x1b[s");
  printf("\x1b[?25l");
  printf("\x1b[2J");
  printf("\x1b[1;1H");
  wprintf(L"key:中文输出");
  printf("\x1b[2;1H");
  wprintf(L"key:中文输出2");
  _getch();
  printf("\x1b[?25h");
  printf("\x1b[u");
  return 0;
}