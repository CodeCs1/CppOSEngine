#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "types.h"
#include "kernel.h"

uint16 stringlen(string2 ch);

class Console {
    public:
        //clear the screen
        void Clear();
        //write a character
        void WriteChr(char c);
        //write a text (with line)
        void WriteLine(string2 ch);
        //read a string
        string ReadLine();
        //wrte a text
        void Write(string2 ch);
};

class ConsoleColor {
    public:
        void BackgroundColor(int text_color,int bg_color);
        void BackgroundColorFromColorCode(int color_code);
        void WriteLine(string2 ch,int text_color,int bg_color);
        void ForegroundColor(int text_color);
};

class Hex {
    public:
        void printfHex(uint8 key);
        void printfHex16(uint16 key);
        void printfHex32(uint32 key);
};



#endif