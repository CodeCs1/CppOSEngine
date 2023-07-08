#include "include/console.h"
#include "include/types.h"
#include "include/string.h"
#include "include/utils.h"

int cursorX;
int cursorY;
bool is_caplock_is_pressed;
bool is_shift_keys_is_pressed;
const uint8 sw=80, sh=25, sd=2;


void ClearFromLineToLine(uint8 from, uint8 to) {
    string vidmem=(string)0xb8000;
    for(uint16 i = sw * from * sd;i<(sw*(to+1)*sd);i++)
    {
        vidmem[i]=0x0;
    }
}
void updateCursor() {
    unsigned tmp;
    tmp = cursorY * sw + cursorX;
    Assembly asm1;
    asm1.out(0x3D4, 14);
    asm1.out(0x3D5, tmp>>8);
    asm1.out(0x3D4, 15);
    asm1.out(0x3D5, tmp);
}
void scrollUp(uint8 lineNum) {
    string vidmem = (string)0xb8000;
    for (uint16 i = 0; i < sw*(sh-1)*sd;i++) {
        vidmem[i] = vidmem[i+sw*2*lineNum];
    }
    ClearFromLineToLine(sh-1-lineNum, sh-1);
    if ((cursorY-lineNum) < 0) {
        cursorX=0;
        cursorY=0;
    }else {
        cursorY -= lineNum;
    }
    updateCursor();
}
void newLineCheck() {
    if (cursorY >=sh-1) {
        scrollUp(1);
    }
}

uint16 stringlen(string2 ch) {
    uint16 i = 1;
    while(ch[i++]);
    return --i;
}

void Console::Clear() {
    ClearFromLineToLine(0, sh-1);
    cursorX=0;
    cursorY=0;
    updateCursor();
}

/*
//read a key from keybroad
//USAGE: readkeys('<character text>');

string readkeys(char text)
{
    Utils utils;
    Console console;
    uint8 i;
    string buffer = (string)utils.malloc(200);
    char text1 = text;
    console.WriteChr(text1);
    buffer[i]=text1;
    i++;
    buffer[i-1]=0;
    return buffer;
}
*/
string Console::ReadLine() {
    Utils utils;
    char buff;
        Assembly _asm_;
        string buffer = (string)utils.malloc(200);
        uint8 read=1;
        uint8 i =0;
        while(read) {
            if (_asm_.in(0x64) & 0x1) {
                switch(_asm_.in(0x60)) {
                    case 0x01: //ESC key
                        WriteLine("ESC Key pressed");
                        buffer[i]=(char)27;
                        i++;
                        break;
                    case 2:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('1'); 
                            buffer[i]='1';
                            i++;
                            break;
                        } else {
                            WriteChr('!');
                            buffer[i]=(char)33;
                            i++;
                            break;
                        }
                    case 3:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('2');
                            buffer[i]='2';
                            i++;
                            break;
                        } else {
                            WriteChr('@');
                            buffer[i]='@';
                            i++;
                            break;
                        }
                        break;
                    case 4:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('3');
                            buffer[i]='3';
                            i++;
                        break;
                        } else {
                            Write("#");
                            buffer[i]='#';
                            i++;
                            break;
                        }
                        break;
                    case 5:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('4');
                            buffer[i]='4';
                            i++;
                        break;
                        } else {
                            WriteChr((char)36);
                            buffer[i]=(char)36;
                            i++;
                            break;
                        }
                        break;
                    case 6:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('5');
                            buffer[i]='5';
                            i++;
                        break;
                        } else {
                            WriteChr('%');
                            buffer[i]='%';
                            i++;
                            
                            break;
                        }
                        break;
                    case 7:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('6');
                            buffer[i]='6';
                            i++;
                        break;
                        } else {
                            WriteChr('^');
                            buffer[i]='^';
                            i++;
                            break;
                        }
                        break;
                    case 8:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('7');
                            buffer[i]='7';
                            i++;
                        break;
                        } else {
                            WriteChr('&');
                            buffer[i]='&';
                            i++;
                            
                            break;
                        }
                        break;
                    case 9:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('8');
                            buffer[i]='8';
                            i++;
                        break;
                        } else {
                            WriteChr('*');
                            buffer[i]='*';
                            i++;
                            
                            break;
                        }
                        break;
                    case 0x0A:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('9');
                            buffer[i]='9';
                            i++;
                        break;
                        } else {
                            WriteChr('(');
                            buffer[i]='(';
                            i++;
                            
                            break;
                        }
                        break;
                    case 11:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('0');
                            buffer[i]='0';
                            i++;
                        break;
                        } else {
                            WriteChr(')');
                            buffer[i]=')';
                            i++;
                            break;
                        }
                        break;
                    case 12:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('-');
                            buffer[i]='-';
                            i++;
                            break;
                        } else {
                            WriteChr('_');
                            buffer[i]='_';
                            i++;
                            break;  
                        }
                        break;
                    case 13:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('=');
                            buffer[i]='=';
                            i++;
                            break;
                        } else {
                            WriteChr('+');
                            buffer[i]='+';
                            i++;
                            break;  
                        }
                        break;
                    case 14:
                        WriteChr('\b');
                        i--;
                        buffer[i]=0;
                        break;
                    case 15:
                        WriteChr('\t');
                        i++;
                        buffer[i]=0;
                        break;
                    case 16:
                        if (!is_caplock_is_pressed) {
                            WriteChr('q');
                            buffer[i]='q';
                            i++;
                            break;
                        } else {
                            WriteChr('Q');
                            buffer[i]='Q';
                            i++;
                            break;            
                        }
                    case 17:
                        if (!is_caplock_is_pressed) {
                            WriteChr('w');
                            buffer[i]='w';
                            i++;
                            break;
                        } else {
                            WriteChr('W');
                            buffer[i]='W';
                            i++;                     
                            break;
                        }
                    case 18:
                        if (!is_caplock_is_pressed) {
                            WriteChr('e');   
                            buffer[i]='e';
                            i++;                     
                            break;
                        } else {
                            WriteChr('E');
                            buffer[i]='E';
                            i++;
                            break;
                        }
                    case 19:
                        if (!is_caplock_is_pressed) {
                            WriteChr('r');   
                            buffer[i]='r';
                            i++;                     
                            break;
                        } else {
                            WriteChr('R');
                            buffer[i]='R';
                            i++;
                            break;
                        }                        
                        break;
                    case 20:
                        if (!is_caplock_is_pressed) {
                            WriteChr('t');
                            buffer[i]='t';
                            i++;                       
                            break;
                        } else {
                            WriteChr('T');
                            buffer[i]='T';
                            i++;
                            break;
                        }
                        break;
                    case 21:
                        if (!is_caplock_is_pressed) {
                            WriteChr('y');
                            buffer[i]='y';
                            i++;                
                            break;
                        } else {
                            WriteChr('Y');
                            buffer[i]='Y';
                            i++;
                            break;
                        }
                        break;
                    case 22:
                        if (!is_caplock_is_pressed) {
                            WriteChr('u');
                            buffer[i]='u';
                            i++;                      
                            break;
                        } else {
                            WriteChr('U');
                            buffer[i]='U';
                            i++;
                            break;
                        }
                        break;
                    case 23:
                        if (!is_caplock_is_pressed) {
                            WriteChr('i');
                            buffer[i]='i';
                            i++;
                            break;
                        } else {
                            WriteChr('I');
                            buffer[i]='I';
                            i++;
                            break;
                        }
                        break;
                    case 24:
                        if (!is_caplock_is_pressed) {
                            WriteChr('o'); 
                            buffer[i]='o';
                            i++;                       
                            break;
                        } else {
                            WriteChr('O');
                            buffer[i]='O';
                            i++;
                            break;
                        }
                        break;
                    case 25:
                        if (!is_caplock_is_pressed) {
                            WriteChr('p');
                            buffer[i]='p';
                            i++;                       
                            break;
                        } else {
                            WriteChr('P');
                            buffer[i]='P';
                            i++;
                            break;
                        }
                        break;
                    case 26:
                        if (!is_caplock_is_pressed) {
                            WriteChr('[');
                            buffer[i]='[';
                            i++;                     
                            break;
                        } else {
                            WriteChr('{');
                            buffer[i]='{';
                            i++;
                            break;
                        }
                        break;
                    case 27:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr(']');
                            buffer[i]=']';
                            i++;                       
                            break;
                        } else {
                            WriteChr('}');
                            buffer[i]='}';
                            i++;
                            break;
                        }
                        break;
                    case 28: //Enter
                        WriteChr('\n');
                        buffer[i]='\n';
                        i++;
                        read=0;
                        break;
                    case 29: //left control
                        //Still in development.
                        break;
                    case 30: //A
                        if (!is_caplock_is_pressed) {
                            WriteChr('a');
                            buffer[i]='a';
                            i++;
                            break;
                        } else {
                            WriteChr((char)65);
                            buffer[i]=(char)65;
                            i++;
                            break;
                        }
                        break;
                    case 31:
                        if (!is_caplock_is_pressed) {
                            WriteChr('s');
                            buffer[i]='s';
                            i++;
                            break;
                        } else {
                            WriteChr('S');
                            buffer[i]='S';
                            i++;
                            break;
                        }
                        break;
                    case 32:
                        if (!is_caplock_is_pressed) {
                            WriteChr('d');
                            buffer[i]='d';
                            i++;
                            break;
                        } else {
                            WriteChr('D');
                            buffer[i]='D';
                            i++;
                            break;
                        }
                        break;
                    case 33:
                        if (!is_caplock_is_pressed) {
                            WriteChr('f');
                            buffer[i]='f';
                            i++;
                            break;
                        } else {
                            WriteChr('F');
                            buffer[i]='F';
                            i++;
                            break;
                        }
                        break;
                    case 34:
                        if (!is_caplock_is_pressed) {
                            WriteChr('g');
                            buffer[i]='g';
                            i++;
                            break;
                        } else {
                            WriteChr('G');
                            buffer[i]='G';
                            i++;
                            break;
                        }
                        break;
                    case 35:
                        if (!is_caplock_is_pressed) {
                            WriteChr('h');
                            buffer[i]='h';
                            i++;
                            break;
                        } else {
                            WriteChr('H');
                            buffer[i]='H';
                            i++;
                            break;
                        }
                        break;
                    case 36:
                        if (!is_caplock_is_pressed) {
                            WriteChr('j');
                            buffer[i]='j';
                            i++;
                            break;
                        } else {
                            WriteChr('J');
                            buffer[i]='J';
                            i++;
                            break;
                        }
                        break;
                    case 37:
                        if (!is_caplock_is_pressed) {
                            WriteChr('k');
                            buffer[i]='k';
                            i++;
                            break;
                        } else {
                            WriteChr('K');
                            buffer[i]='K';
                            i++;
                            break;
                        }
                        break;
                    case 38:
                        if (!is_caplock_is_pressed) {
                            WriteChr('l');
                            buffer[i]='l';
                            i++;
                            break;
                        } else {
                            WriteChr('L');
                            buffer[i]='L';
                            i++;
                            break;
                        }
                        break;
                    case 39:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr(';');
                            buffer[i]=';';
                            i++;
                            break;
                        } else {
                            WriteChr(':');
                            buffer[i]=':';
                            i++;
                            is_shift_keys_is_pressed = false;
                            break;
                        }
                        break;
                    case 40:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr((char)39); //39 ""
                            buffer[i]=(char)39;
                            i++;
                            break;
                        } else {
                            WriteChr((char)34); ///34 ''
                            buffer[i]=(char)34;
                            i++;
                            break;
                        }
                    case 41: //~/`
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('`');
                            buffer[i]='`';
                            i++;
                            break;
                        } else {
                            WriteChr('~');
                            buffer[i]='~';
                            i++;
                            break;
                        }
                        break;
                    case 43:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('\\');
                            buffer[i]='\\';
                            i++;
                            break;
                        } else {
                            WriteChr('|');
                            buffer[i]='|';
                            i++;
                            break;
                        }
                        break;
                    case 44:
                        if (!is_caplock_is_pressed) {
                            WriteChr('z');
                            buffer[i]='z';
                            i++;
                            break;
                        } else {
                            WriteChr('Z');
                            buffer[i]='Z';
                            i++;
                            break;
                        }
                        break;
                    case 45:
                        if (!is_caplock_is_pressed) {
                            WriteChr('x');
                            buffer[i]='x';
                            i++;
                            break;
                        } else {
                            WriteChr('X');
                            buffer[i]='X';
                            i++;
                            break;
                        }
                        break;
                    case 46:
                        if (!is_caplock_is_pressed) {
                            WriteChr('c');
                            buffer[i]='c';
                            i++;
                            break;
                        } else {
                            WriteChr('C');
                            buffer[i]='C';
                            i++;
                            break;
                        }
                        break;
                    case 47:
                        if (!is_caplock_is_pressed) {
                            WriteChr('v');
                            buffer[i]='v';
                            i++;
                            break;
                        } else {
                            WriteChr('V');
                            buffer[i]='V';
                            i++;
                            break;
                        }
                        break;
                    case 48:
                        if (!is_caplock_is_pressed) {
                            WriteChr('b');
                            buffer[i]='b';
                            i++;
                            break;
                        } else {
                            WriteChr('B');
                            buffer[i]='B';
                            i++;
                            break;
                        }
                        break;
                    case 49:
                        if (!is_caplock_is_pressed) {
                            WriteChr('n');
                            buffer[i]='n';
                            i++;
                            break;
                        } else {
                            WriteChr('N');
                            buffer[i]='N';
                            i++;
                            break;
                        }
                        break;
                    case 50:
                        if (!is_caplock_is_pressed) {
                            WriteChr('m');
                            buffer[i]='m';
                            i++;
                            break;
                        } else {
                            WriteChr('M');
                            buffer[i]='M';
                            i++;
                            break;
                        }
                        break;
                    case 51:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr(',');
                            buffer[i]=',';
                            i++;
                            break;
                        } else {
                            WriteChr('<');
                            buffer[i]='<';
                            i++;
                            break;
                        }
                        break;
                    case 52:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('.');
                            buffer[i]='.';
                            i++;
                            break;
                        } else {
                            WriteChr('>');
                            buffer[i]='>';
                            i++;
                            break;
                        }
                        break;
                    case 53:
                        if (!is_shift_keys_is_pressed) {
                            WriteChr('?');
                            buffer[i]='?';
                            i++;
                            break;
                        } else {
                            WriteChr('/');
                            buffer[i]='/';
                            i++;
                            break;
                        }
                        break;
                    case 42:
                    case 54: //shift pressed
                        is_shift_keys_is_pressed = true;
                        i++;
                        break;

                    case 57:
                        WriteChr((char)32);
                        buffer[i]=(char)32;
                        i++;
                        break;
                    case 58:
                        if (!is_caplock_is_pressed) {
                            is_caplock_is_pressed = true;
                        } else {
                            is_caplock_is_pressed=false;
                        }
                        i++;
                        break;
                    case 170:
                    case 182:
                        is_shift_keys_is_pressed=false;
                        i++;
                        break;
                    //special button
                    case (0xE0,0x4B): //left
                        cursorX--;
                        updateCursor();
                        break;
                    case (0xE0, 0x4D): //right
                        cursorX++;
                        updateCursor();
                        break;
                    }
                        

                }
            }
            buffer[i-1] = 0;
            return buffer;
}
void Console::WriteChr(char c) {
    string vidmem = (string)0xb8000;
        switch(c) {
            case (0x08):
                if (cursorX > 0) {
                    cursorX--;
                    vidmem[(cursorY*sw+cursorX)*sd]=0x00;
                }
                break;
            case (0x09):
                cursorX=(cursorX+8) & ~(8 - 1);
                break;
            case ('\r'):
                cursorX=0;
                break;
            case ('\n'):
                cursorX=0;
                cursorY++;
                break;
            default:
                vidmem[((cursorY*sw+cursorX))*sd] = c;
                vidmem[((cursorY*sw+cursorX))*sd+1]=0x0F;
                cursorX++;
                break;
        }
        if(cursorX >= sw){
            cursorX=0;
            cursorY++;
        }
        newLineCheck();
        updateCursor();
}


void Console::WriteLine(string2 ch) {
    for(uint16 i=0; i<stringlen(ch);i++) {
        WriteChr(ch[i]);
    }
        cursorX=0;
        cursorY++;
}
void Console::Write(string2 ch) {
    for(uint16 i=0; i<stringlen(ch);i++) {
        WriteChr(ch[i]);
    }
}

int color=0x0F;

void ConsoleColor::BackgroundColor(int text_color, int bg_color) {
    color= (bg_color << 4) | text_color;
}

void ConsoleColor::BackgroundColorFromColorCode(int color_code) {
    color = color_code;
}

void ConsoleColor::WriteLine(string2 ch, int text_color, int bg_color) {
    Console console;
    int current = color;
    BackgroundColor(text_color, bg_color);
    console.WriteLine(ch);
    BackgroundColorFromColorCode(current);
}

void ConsoleColor::ForegroundColor(int text_color) {
    BackgroundColor(text_color, 0);
}

void Hex::printfHex(uint8 key) {
    Console console;
    string text = "00";
    string hex="0123456789ABCDEF";
    text[0]=hex[(key>>4)&0xF];
    text[1]=hex[key&0xF];
    console.Write(text);
}

void Hex::printfHex16(uint16 key) {
    printfHex((key>>8)&0xFF);
    printfHex(key&0xFF);
}

void Hex::printfHex32(uint32 key) {
    printfHex((key>>24)&0xFF);
    printfHex((key>>16)&0XFF);
    printfHex((key>>8)&0XFF);
    printfHex(key&0XFF);
}