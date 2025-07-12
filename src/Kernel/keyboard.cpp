#include "include/driver/Keyboard.h"
#include "console.h"
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
	
}

void Keyboard::Init() {
  InterruptRequiests::Init_Custom_Handler(1, keybroad_handler);
}

bool is_shift_key_pressed, is_caplock_pressed;

uint16_t Keyboard::get_keys() {
	uint16_t c = 0xff;
    uint8_t is_press = 0;
	while(!is_press) {
	if(Assembly::in(0x64) & 0x1) {
      	uint8_t keypressindex = Assembly::in(0x60);
      	switch (keypressindex) {
			case 0xe0:
				c=(keypressindex << 8) | Assembly::in(0x60);
				break;
        	case 0x01:
          		break;
        	case 0x2a: // shift pressed
        	case 0x36:
          		is_shift_key_pressed = true;
          	break;
        	case 0xAA: // shift released
        	case 0xB6:
          		is_shift_key_pressed = false;
          	break;
        	case 58:
          	if (!is_caplock_pressed) {
        	    is_caplock_pressed = true;
    	      } else {
	            is_caplock_pressed = false;
          	}
          	break;
        	case 0x39:
          		c=0x20;
          	break;
        	case 0x1C:
          		c='\n';
          		break;
        	default:
          		if (kbdus[keypressindex] != 0 && keypressindex <= 0x58) {
            		if (is_shift_key_pressed || is_caplock_pressed) {
              		c=kbdus[keypressindex]-32;
            		} else {
              			c=kbdus[keypressindex];
            		}
        	  	}
    	      	break;
	      	}
			is_press=1;
    	}
	}
	return c;
}
