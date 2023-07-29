#ifndef __SPORT_H__
#define __SPORT_H__

//Support 5 ports
#include "types.h"
#define _COM1_ 0x3F8
#define _COM2_ 0x2F8
#define _COM3_ 0x3E8
#define _COM4_ 0x2E8
#define _COM5_ 0x5F8
#define _COM6_ 0x4F8
#define _COM7_ 0x5E0
#define _COM8_ 0x4E0

enum PORTTYPE {
    COM1 = _COM1_,
    COM2 = _COM2_,
    COM3 = _COM3_,
    COM4 = _COM4_,
    COM5 = _COM5_,
    COM6 = _COM6_,
    COM7 = _COM7_,
    COM8 = _COM8_,
};

class Port {
    protected:
        PORTTYPE port;
    public:
        Port(PORTTYPE port);
        ~Port();

        bool Init_Port();
        int received();
        char read_serial();
        bool is_transmit_empty();
        void write(char a);
        void writeline(string2 a);
};

#endif