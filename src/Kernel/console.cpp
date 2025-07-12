#include <console.h>
#include <driver/Keyboard.h>
#include <mem.h>
#include <string.h>
#include <types.h>
#include <pmm.h>

int cursorX;
int cursorY;
const uint8 sw = 80, sh = 25, sd = 2;

uint16_t* vidmem = (uint16_t*)0xb8000;
int color = 0x0F;

void ClearFromLineToLine(uint8 from, uint8 to) {
  Console console;
  string vidmem = (string)0xb8000;
  for (uint16 i = sw * from * sd; i < (sw * (to + 1) * sd); i++) {
    vidmem[i] = 0x0;
  }
}
void updateCursor() {
  unsigned tmp=cursorY * sw + cursorX;
  Assembly::out(0x3D4, 14);
  Assembly::out(0x3D5, tmp >> 8);
  Assembly::out(0x3D4, 15);
  Assembly::out(0x3D5, tmp);
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
  if (cursorY >= sh - 1) {
    scrollUp(1);
  }
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	Assembly::out(0x3D4, 0x0A);
	Assembly::out(0x3D5, (Assembly::in(0x3D5) & 0xC0) | cursor_start);

	Assembly::out(0x3D4, 0x0B);
	Assembly::out(0x3D5, (Assembly::in(0x3D5) & 0xE0) | cursor_end);
}


void Console::Init() {
	enable_cursor(0, 15);
}

void Console::Clear() {
  ClearFromLineToLine(0, sh - 1);
  cursorX = 0;
  cursorY = 0;
  updateCursor();
}

void Console::WriteChr(char c) {

  switch (c) {
  case '\b':
    if (cursorX > 0) {
      cursorX--;
      vidmem[cursorY * sw + cursorX] = (color << 8) | ' ';
    }
    break;
  case (0x09):
    cursorX = (cursorX + 8) & ~(8 - 1);
  	break;
  case '\r':
    cursorX = 0;
    break;
  case '\n':
    cursorX = 0;
    cursorY++;
    break;
  default:
    vidmem[cursorY * sw + cursorX] = (color << 8) | c;
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
  for (int i = 0; i < strlen(text); i++) {
    WriteChr(text[i]);
  }
  cursorY++;
  cursorX = 0;
  updateCursor();
}
void Console::Write(string2 text) {
  for (int i = 0; i < strlen(text); i++) {
    WriteChr(text[i]);
  }
}

#define LSH_RL_BUFSIZE 1024
#define EOF (-1)


string Console::ReadLine() {
	uint8_t read = 1;
	char* buffer=(char*)PMM::Alloc(32);
	int i=0;

	const uint8_t x_begin = cursorX;
	const uint8_t y_begin = cursorY;

	uint8_t x_current = cursorX;
	uint8_t y_current = cursorY;

	while(read) {
		uint16_t c= Keyboard::get_keys();
		switch (c) {
		case 0xff:
			break;
		case 0xe04b: // cursor left
			if (i>0) {
				i--;
				cursorX--;
				updateCursor();
			}
			break;
		case 0xe04d: //cursor right
			if (i < strlen(buffer)) {
				i++;
				cursorX++;
				updateCursor();
			}
			break;
		case '\b':
			i--;
			Console::WriteChr('\b');
			buffer[i]=c;
			x_current--;
			break;
		case 0xe047: // Home key
			i=0;
			cursorX=x_begin;
			cursorY=y_begin;
			updateCursor();
			break;
		case 0xe04f: // end key
			i=strlen(buffer);
			cursorX = x_current;
			cursorY=y_current;
			updateCursor();
			break;
		case '\n':
		case 0xe01c: // enter keypad
			Console::WriteChr('\n');
			read = 0;
			break;
		default:
			if (c >= 0x20 && c <= 0x7e) {
				WriteChr(c);
				buffer[i]= c;
				i++;
				x_current++;
			}
			break;
		}
	}
	return buffer;
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
  string text;
  string2 hex = "0123456789ABCDEF";
  text[0] = hex[(key >> 4) & 0xF];
  text[1] = hex[key & 0xF];
  Console::Write(text);
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
