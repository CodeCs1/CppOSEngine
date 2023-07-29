/*Mouse code by SANiK
https://forum.osdev.org/viewtopic.php?t=10247*/

#include "../include/Graphics/mouse.h"
#include "../include/kernel.h"
#include "../include/types.h"

uint8 buffer[3];
uint8 offset;
uint8 buttons;

int32 x,y;

uint8 mouse_cycle=0;

void handler() {
    Assembly _asm_;
    switch(mouse_cycle) {
        case 0:
            buffer[0]= _asm_.in(0x60);
            mouse_cycle++;
            break;
        case 1:
            buffer[1]=_asm_.in(0x60);
            mouse_cycle++;
            break;
        case 2:
            buffer[2]=_asm_.in(0x60);
            x=buffer[1];
            y=buffer[2];
            mouse_cycle=0;
            break;
    }
}

void mouse_wait(uint8 a_type) //unsigned char
{
    Assembly _asm_;
    uint32 _time_out=100000; //unsigned int
    if(a_type==0)
    {
        while(_time_out--) //Data
        {
            if((_asm_.in(0x64) & 1)==1)
            {
                return;
            }
    }
    return;
    }
    else
    {
        while(_time_out--) //Signal
        {
            if((_asm_.in(0x64) & 2)==0)
            {
                return;
            }
        }
        return;
    }
}

void mouse_write(uint8 a_write) //unsigned char
{
    Assembly _asm_;
  //Wait to be able to send a command
    mouse_wait(1);
  //Tell the mouse we are sending a command
    _asm_.out(0x64, 0xD4);
  //Wait for the final part
    mouse_wait(1);
  //Finally write
    _asm_.out(0x60, a_write);
}

uint8 mouse_read()
{
    Assembly _asm_;
  //Get's response from mouse
    mouse_wait(0);
    return _asm_.in(0x60);
}

uint32 Mouse::MouseEnable() {
    uint8 _status;
    Assembly _asm_;

    mouse_wait(1);
    _asm_.out(0x64, 0xAB);

    mouse_wait(1);
    _asm_.out(0x64, 0x20);

    mouse_wait(0);
    _status=(_asm_.in(0x60)|2);

    mouse_wait(1);
    _asm_.out(0x64, 0x60);

    mouse_wait(1);
    _asm_.out(0x64,_status);

    mouse_write(0xF6);
    mouse_read();

    mouse_write(0xF4);
    mouse_read();

    handler();
    return 0;
}