#include "include/console.h"
#include "include/driver/Keybroad.h"
#include "include/string.h"
#include "include/types.h"
#include "include/utils.h"

int cursorX;
int cursorY;
const uint8 sw = 80, sh = 25, sd = 2;

uint8 *textmemptr;
int color = 0x0F;

void ClearFromLineToLine(uint8 from, uint8 to) {
  Console console;
  string vidmem = (string)0xb8000;
  for (uint16 i = sw * from * sd; i < (sw * (to + 1) * sd); i++) {
    vidmem[i] = 0x0;
  }
}
void updateCursor() {
  unsigned tmp;
  Console console;
  tmp = cursorY * sw + cursorX;
  Assembly asm1;
  asm1.out(0x3D4, 14);
  asm1.out(0x3D5, tmp >> 8);
  asm1.out(0x3D4, 15);
  asm1.out(0x3D5, tmp);
}

void Console::updateCursor2(int X, int Y) {
  cursorX = X;
  cursorY = Y;
  updateCursor();
}

void scrollUp(uint8 lineNum) {

  Console console;

  string vidmem = (string)0xb8000;
  for (uint16 i = 0; i < sw * (sh - 1) * 2; i++) {
    vidmem[i] = vidmem[i + sw * 2 * lineNum];
  }
  ClearFromLineToLine(sh - 1 - lineNum, sh - 1);
  if ((cursorY - lineNum) < 0) {
    cursorX = 0;
    cursorY = 0;
  } else {
    cursorY -= lineNum;
  }
  updateCursor();
}
void newLineCheck() {
  Console console;
  if (cursorY >= sh - 1) {
    scrollUp(1);
  }
}

void Console::Clear() {
  ClearFromLineToLine(0, sh - 1);
  cursorX = 0;
  cursorY = 0;
  updateCursor();
}

void Console::WriteChr(char c) {

  Console console;

  volatile char *vidmem = (volatile char *)0xb8000;

  switch (c) {
  case (0x08):
    if (cursorX > 0) {
      cursorX--;
      vidmem[(cursorY * sw + cursorX) * 2] = 0;
    }
    break;
  case (0x09):
    cursorX = (cursorX + 8) & ~(8 - 1);
  case ('\r'):
    cursorX = 0;
    break;
  case ('\n'):
    cursorX = 0;
    cursorY++;
    break;
  default:
    vidmem[((cursorY * sw + cursorX)) * 2] = c;
    vidmem[((cursorY * sw + cursorX)) * 2 + 1] = color;
    cursorX++;
    break;
  }
  if (cursorX >= 80) {
    cursorX = 0;
    cursorY++;
  }
  newLineCheck();
  updateCursor();
}

void Console::WriteLine(string2 text) {
  for (int i = 0; i < stringlen(text); i++) {
    WriteChr(text[i]);
  }
  cursorY++;
  cursorX = 0;
}
void Console::Write(string2 text) {
  for (int i = 0; i < stringlen(text); i++) {
    WriteChr(text[i]);
  }
}

#define LSH_RL_BUFSIZE 1024
#define EOF (-1)

string Console::ReadLine() {
  return Keybroad::get_keys();
}

void Console::BackgroundColor(ConsoleColor text_color, ConsoleColor bg_color) {
  color = (bg_color << 4) | text_color;
}
// Use int
void Console::BackgroundColor(int text_color, ConsoleColor bg_color) {
  color = (bg_color << 4) | text_color;
}

void Console::BackgroundColorFromColorCode(int color_code) {
  color = color_code;
}

void Console::WriteLine(string2 ch, ConsoleColor text_color,
                        ConsoleColor bg_color) {
  Console console;
  int current = color;
  BackgroundColor(text_color, bg_color);
  console.WriteLine(ch);
  BackgroundColorFromColorCode(current);
}

void Console::ForegroundColor(ConsoleColor text_color) {
  BackgroundColor(text_color, ConsoleColor::Black);
}

void Console::WriteChr(uint8 ch, ConsoleColor text_color,
                       ConsoleColor bg_color) {
  uint16 attrib = (bg_color << 4) | (text_color & 0x0F);
  Console console;
  console.WriteChr(ch);
}

void Hex::printfHex(uint8 key) {
  Console console;
  string text = "00";
  string hex = "0123456789ABCDEF";
  text[0] = hex[(key >> 4) & 0xF];
  text[1] = hex[key & 0xF];
  console.Write(text);
}

void Hex::printfHex16(uint16 key) {
  printfHex((key >> 8) & 0xFF);
  printfHex(key & 0xFF);
}

void Hex::printfHex32(uint32 key) {
  printfHex((key >> 24) & 0xFF);
  printfHex((key >> 16) & 0XFF);
  printfHex((key >> 8) & 0XFF);
  printfHex(key & 0XFF);
}

void Console::WriteLine2(string2 ch) {
  volatile char *video = (volatile char *)0xB8000;
  while (*ch != 0) {
    *video++ = *ch++;
    *video++ = 0x0F;
  }
}