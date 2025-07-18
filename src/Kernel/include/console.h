#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "types.h"
#include "kernel.h"

#define SPECIALKEY 0xE0

enum ConsoleColor {
    Black=0,
    Blue=1,
    Green=2,
    Cyan=3,
    Red=4,
    Magenta=5,
    Brown=6,
    LightGrey=7,
    DarkGrey=8,
    LightBlue=9,
    LightGreen=10,
    LightCyan=11,
    LightRed=12,
    LightMagenta=13,
    Yellow=14,
    White=15
};

class Console {
    private:
        int ScreenX=80;
        int ScreenY=25;
    public:
        //Initialize console
        static void Init();
        //clear the screen
        static void Clear();
        //write a character
        static void WriteChr(char c);
        //write a text (with line)
        static void WriteLine(string2 ch);
        //read a string
        static string ReadLine();
        //wrte a text
        static void Write(string2 ch);

        //get the columns of the display
        int GetScreenX() {
            return ScreenX;
        }
        //get the rows of the display
        int GetScreenY() {
            return ScreenY;
        }
        static void BackgroundColor(ConsoleColor text_color,ConsoleColor bg_color);
        static void BackgroundColor(int text_color,ConsoleColor bg_color);
        void BackgroundColorFromColorCode(int color_code);
        void WriteLine(string2 ch,ConsoleColor text_color,ConsoleColor bg_color);
        static void ForegroundColor(ConsoleColor text_color);
        void WriteChr(uint8 ch, ConsoleColor text_color, ConsoleColor bg_color);
};

//Other keys like up, down, left, right, win (super), numpad, ...
class ConsoleKey {
    public:
        virtual void KeyPress() {}
};

class Hex {
    public:
        static void printfHex(uint8 key);
        static void printfHex16(uint16 key);
        static void printfHex32(uint32 key);
};


#endif
