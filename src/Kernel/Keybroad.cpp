#include "include/Core.h"
#include "include/driver/Keybroad.h"
#include "include/PIC.h"
#include "include/console.h"
#include "include/kernel.h"
#include "include/IDT.h"

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

extern "C" void keybroad_handler(struct regs* r);

extern "C" void keybroad_handler(struct regs* r) {
  Console::WriteLine("Loaded");
}

void Keybroad::Init() {
  Console::WriteLine("Loading Keybroad...");
  InterruptRequiests::Init_Custom_Handler(1, keybroad_handler);
}

bool is_shift_keys_is_pressed, is_caplock_is_pressed;

int X, Y;

void updateCursor2() {
  int cursorX=X;
  int cursorY=Y;
  const uint8 sw = 80, sh = 25, sd = 2;
  unsigned tmp;
  Console console;
  tmp = cursorY * sw + cursorX;
  Assembly asm1;
  asm1.out(0x3D4, 14);
  asm1.out(0x3D5, tmp >> 8);
  asm1.out(0x3D4, 15);
  asm1.out(0x3D5, tmp);
}

string Keybroad::get_keys() {
  int cursorX=X;
  int cursorY=Y;
  const uint8 sw = 80, sh = 25, sd = 2;
  Utils utils;
  char buff;
  Assembly _asm_;
  string buffer = (string)utils.malloc(200);
  uint8 read = 1;
  uint8 i = 0;
  while (read) {
    if (_asm_.in(0x64) & 0x1) {
      uint8_t keypressindex = _asm_.in(0x60);
      switch (keypressindex) {
        case 0x01:
          break;
        case 0x2a: // shift pressed
        case 0x36:
          is_shift_keys_is_pressed = true;
          i++;
          break;
        case 0xAA: // shift released
        case 0xB6:
          is_shift_keys_is_pressed = false;
          i++;
          break;
        case 58:
          if (!is_caplock_is_pressed) {
            is_caplock_is_pressed = true;
          } else {
            is_caplock_is_pressed = false;
          }
          i++;
          break;
        case 0x39:
          Console::WriteChr((char)32);
          buffer[i] = (char)32;
          i++;
          break;
        case 0x1C:
          Console::WriteChr('\n');
          buffer[i] = '\n';
          read=0;
          i++;
          break;
        default:
          if (kbdus[keypressindex] != 0 && keypressindex <= 0x58) {
            if (is_shift_keys_is_pressed) {
              Console::WriteChr(kbdus[keypressindex] - 32);
              buffer[i] = kbdus[keypressindex] - 32;
            } else {
              Console::WriteChr(kbdus[keypressindex]);
              buffer[i] = kbdus[keypressindex];
            }
            i++;
          }
          break;        
      }
    }
  }
  updateCursor2();
  buffer[i - 1] = 0;
  return buffer;
}