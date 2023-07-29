#ifndef __DEBUG_PORT__
#define __DEBUG_PORT__


#include "../Core.h"
#include "../SPort.h"
inline void EnableDebug() {
    Console console;
    Port debug_port(PORTTYPE::COM1);
    int X;
    int Y;
    int w=80;
    int h=25;
    X = w/4;
    Y=  h/5;
    console.updateCursor2(X, Y);
    console.ForegroundColor(ConsoleColor::Green);
    console.WriteLine("Waiting for g++ debugger connection...");
    debug_port.Init_Port();
    debug_port.read_serial();
    debug_port.write('H');
    debug_port.write('E');
    debug_port.write('L');
    debug_port.write('L');
    debug_port.write('O');
    debug_port.read_serial();
    if (debug_port.received() == 0) {
        console.WriteLine("Connected successfully.");
        __asm__("int3");
        return;
    } else {
        console.ForegroundColor(ConsoleColor::Red);
        console.WriteLine("Conenction failed.\nPlease reconnect COM1 and try again.");
        __asm__("hlt");
    }
}

#endif