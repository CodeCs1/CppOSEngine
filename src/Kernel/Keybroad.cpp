#include "include/Core.h"
#include "include/Keybroad.h"
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

#pragma region L_O_N_G_code

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
      switch (_asm_.in(0x60)) {
      case 0x01: // ESC key
        Console::WriteLine("ESC Key pressed");
        buffer[i] = (char)27;
        i++;
        break;
      case 2:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('1');
          buffer[i] = '1';
          i++;
          break;
        } else {
          Console::WriteChr('!');
          buffer[i] = (char)33;
          i++;
          break;
        }
      case 3:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('2');
          buffer[i] = '2';
          i++;
          break;
        } else {
          Console::WriteChr('@');
          buffer[i] = '@';
          i++;
          break;
        }
        break;
      case 4:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('3');
          buffer[i] = '3';
          i++;
          break;
        } else {
          Console::WriteChr('#');
          buffer[i] = '#';
          i++;
          break;
        }
        break;
      case 5:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('4');
          buffer[i] = '4';
          i++;
          break;
        } else {
          Console::WriteChr((char)36);
          buffer[i] = (char)36;
          i++;
          break;
        }
        break;
      case 6:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('5');
          buffer[i] = '5';
          i++;
          break;
        } else {
          Console::WriteChr('%');
          buffer[i] = '%';
          i++;

          break;
        }
        break;
      case 7:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('6');
          buffer[i] = '6';
          i++;
          break;
        } else {
          Console::WriteChr('^');
          buffer[i] = '^';
          i++;
          break;
        }
        break;
      case 8:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('7');
          buffer[i] = '7';
          i++;
          break;
        } else {
          Console::WriteChr('&');
          buffer[i] = '&';
          i++;

          break;
        }
        break;
      case 9:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('8');
          buffer[i] = '8';
          i++;
          break;
        } else {
          Console::WriteChr('*');
          buffer[i] = '*';
          i++;

          break;
        }
        break;
      case 0x0A:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('9');
          buffer[i] = '9';
          i++;
          break;
        } else {
          Console::WriteChr('(');
          buffer[i] = '(';
          i++;

          break;
        }
        break;
      case 11:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('0');
          buffer[i] = '0';
          i++;
          break;
        } else {
          Console::WriteChr(')');
          buffer[i] = ')';
          i++;
          break;
        }
        break;
      case 12:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('-');
          buffer[i] = '-';
          i++;
          break;
        } else {
          Console::WriteChr('_');
          buffer[i] = '_';
          i++;
          break;
        }
        break;
      case 13:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('=');
          buffer[i] = '=';
          i++;
          break;
        } else {
          Console::WriteChr('+');
          buffer[i] = '+';
          i++;
          break;
        }
        break;
      case 14:
        Console::WriteChr('\b');
        i--;
        break;
      case 15:
        Console::WriteChr('\t');
        i++;
        buffer[i] = 0;
        break;
      case 16:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('q');
          buffer[i] = 'q';
          i++;
          break;
        } else {
          Console::WriteChr('Q');
          buffer[i] = 'Q';
          i++;
          break;
        }
      case 17:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('w');
          buffer[i] = 'w';
          i++;
          break;
        } else {
          Console::WriteChr('W');
          buffer[i] = 'W';
          i++;
          break;
        }
      case 18:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('e');
          buffer[i] = 'e';
          i++;
          break;
        } else {
          Console::WriteChr('E');
          buffer[i] = 'E';
          i++;
          break;
        }
      case 19:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('r');
          buffer[i] = 'r';
          i++;
          break;
        } else {
          Console::WriteChr('R');
          buffer[i] = 'R';
          i++;
          break;
        }
        break;
      case 20:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('t');
          buffer[i] = 't';
          i++;
          break;
        } else {
          Console::WriteChr('T');
          buffer[i] = 'T';
          i++;
          break;
        }
        break;
      case 21:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('y');
          buffer[i] = 'y';
          i++;
          break;
        } else {
          Console::WriteChr('Y');
          buffer[i] = (char)21;
          i++;
          break;
        }
        break;
      case 22:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('u');
          buffer[i] = 'u';
          i++;
          break;
        } else {
          Console::WriteChr('U');
          buffer[i] = 'U';
          i++;
          break;
        }
        break;
      case 23:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('i');
          buffer[i] = 'i';
          i++;
          break;
        } else {
          Console::WriteChr('I');
          buffer[i] = 'I';
          i++;
          break;
        }
        break;
      case 24:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('o');
          buffer[i] = 'o';
          i++;
          break;
        } else {
          Console::WriteChr('O');
          buffer[i] = 'O';
          i++;
          break;
        }
        break;
      case 25:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('p');
          buffer[i] = 'p';
          i++;
          break;
        } else {
          Console::WriteChr('P');
          buffer[i] = 'P';
          i++;
          break;
        }
        break;
      case 26:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('[');
          buffer[i] = '[';
          i++;
          break;
        } else {
          Console::WriteChr('{');
          buffer[i] = '{';
          i++;
          break;
        }
        break;
      case 27:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr(']');
          buffer[i] = ']';
          i++;
          break;
        } else {
          Console::WriteChr('}');
          buffer[i] = '}';
          i++;
          break;
        }
        break;
      case 28: // Enter
        Console::WriteChr('\n');
        buffer[i] = '\n';
        i++;
        read = 0;
        break;
      case 29: // left control
        // Still in development.
        break;
      case 30: // A
        if (!is_caplock_is_pressed) {
          Console::WriteChr('a');
          buffer[i] = 'a';
          i++;
          break;
        } else {
          Console::WriteChr((char)65);
          buffer[i] = (char)65;
          i++;
          break;
        }
        break;
      case 31:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('s');
          buffer[i] = 's';
          i++;
          break;
        } else {
          Console::WriteChr('S');
          buffer[i] = 'S';
          i++;
          break;
        }
        break;
      case 32:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('d');
          buffer[i] = 'd';
          i++;
          break;
        } else {
          Console::WriteChr('D');
          buffer[i] = 'D';
          i++;
          break;
        }
        break;
      case 33:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('f');
          buffer[i] = 'f';
          i++;
          break;
        } else {
          Console::WriteChr('F');
          buffer[i] = 'F';
          i++;
          break;
        }
        break;
      case 34:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('g');
          buffer[i] = 'g';
          i++;
          break;
        } else {
          Console::WriteChr('G');
          buffer[i] = 'G';
          i++;
          break;
        }
        break;
      case 35:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('h');
          buffer[i] = 'h';
          i++;
          break;
        } else {
          Console::WriteChr('H');
          buffer[i] = 'H';
          i++;
          break;
        }
        break;
      case 36:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('j');
          buffer[i] = 'j';
          i++;
          break;
        } else {
          Console::WriteChr('J');
          buffer[i] = 'J';
          i++;
          break;
        }
        break;
      case 37:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('k');
          buffer[i] = 'k';
          i++;
          break;
        } else {
          Console::WriteChr('K');
          buffer[i] = 'K';
          i++;
          break;
        }
        break;
      case 38:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('l');
          buffer[i] = 'l';
          i++;
          break;
        } else {
          Console::WriteChr('L');
          buffer[i] = 'L';
          i++;
          break;
        }
        break;
      case 39:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr(';');
          buffer[i] = ';';
          i++;
          break;
        } else {
          Console::WriteChr(':');
          buffer[i] = ':';
          i++;
          is_shift_keys_is_pressed = false;
          break;
        }
        break;
      case 40:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr((char)39); // 39 ""
          buffer[i] = (char)39;
          i++;
          break;
        } else {
          Console::WriteChr((char)34); /// 34 ''
          buffer[i] = (char)34;
          i++;
          break;
        }
      case 41: //~/`
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('`');
          buffer[i] = '`';
          i++;
          break;
        } else {
          Console::WriteChr('~');
          buffer[i] = '~';
          i++;
          break;
        }
        break;
      case 43:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('\\');
          buffer[i] = '\\';
          i++;
          break;
        } else {
          Console::WriteChr('|');
          buffer[i] = '|';
          i++;
          break;
        }
        break;
      case 44:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('z');
          buffer[i] = 'z';
          i++;
          break;
        } else {
          Console::WriteChr('Z');
          buffer[i] = 'Z';
          i++;
          break;
        }
        break;
      case 45:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('x');
          buffer[i] = 'x';
          i++;
          break;
        } else {
          Console::WriteChr('X');
          buffer[i] = 'X';
          i++;
          break;
        }
        break;
      case 46:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('c');
          buffer[i] = 'c';
          i++;
          break;
        } else {
          Console::WriteChr('C');
          buffer[i] = 'C';
          i++;
          break;
        }
        break;
      case 47:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('v');
          buffer[i] = 'v';
          i++;
          break;
        } else {
          Console::WriteChr('V');
          buffer[i] = 'V';
          i++;
          break;
        }
        break;
      case 48:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('b');
          buffer[i] = 'b';
          i++;
          break;
        } else {
          Console::WriteChr('B');
          buffer[i] = 'B';
          i++;
          break;
        }
        break;
      case 49:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('n');
          buffer[i] = 'n';
          i++;
          break;
        } else {
          Console::WriteChr('N');
          buffer[i] = 'N';
          i++;
          break;
        }
        break;
      case 50:
        if (!is_caplock_is_pressed) {
          Console::WriteChr('m');
          buffer[i] = 'm';
          i++;
          break;
        } else {
          Console::WriteChr('M');
          buffer[i] = 'M';
          i++;
          break;
        }
        break;
      case 51:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr(',');
          buffer[i] = ',';
          i++;
          break;
        } else {
          Console::WriteChr('<');
          buffer[i] = '<';
          i++;
          break;
        }
        break;
      case 52:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('.');
          buffer[i] = '.';
          i++;
          break;
        } else {
          Console::WriteChr('>');
          buffer[i] = '>';
          i++;
          break;
        }
        break;
      case 53:
        if (!is_shift_keys_is_pressed) {
          Console::WriteChr('/');
          buffer[i] = '/';
          i++;
          break;
        } else {
          Console::WriteChr('?');
          buffer[i] = '\?';
          i++;
          break;
        }
        break;
      case 42:
      case 54: // shift pressed
        is_shift_keys_is_pressed = true;
        i++;
        break;

      case 57:
        Console::WriteChr((char)32);
        buffer[i] = (char)32;
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
      case 170:
      case 182:
        is_shift_keys_is_pressed = false;
        i++;
        break;
      // special button
      case (SPECIALKEY, 0x4B): // left
        cursorX--;
        updateCursor2();
        break;
      case (SPECIALKEY, 0x4D): // right
        cursorX++;
        updateCursor2();
        break;
      case (SPECIALKEY, 0x53):
        int current = cursorX;
        cursorX += 1;
        Console::WriteChr('\b');
        cursorX = current;
        break;
      }
    }
  }
  updateCursor2();
  buffer[i - 1] = 0;
  return buffer;
}
#pragma endregion