#include "../include/string.h"
#include "../include/Sound/soundblaster.h"
#include "../include/kernel.h"
#include "../include/mem.h"
#include "../include/console.h"
#include "../include/mem.h"
int channel_number1;

//inthz 1 -> 255
SoundBlasterDriver::SoundBlasterDriver(int channel_number, int channel) {
    Assembly _asm_;
    _asm_.in(MIXERPORT);
    _asm_.in(MIXERDATAPORT);
    _asm_.in(RESET);
    _asm_.in(WRITE);
    _asm_.in(READ_STATUS);
    _asm_.in(INTERRUPT);
    channel_number1 = channel_number;
}

SoundBlasterDriver::~SoundBlasterDriver() { //disable sound driver
    Assembly _asm_;
    _asm_.out(WRITE, 0xD3);
}

void SoundBlasterDriver::Reset() {
    Assembly _asm_;
    _asm_.out(RESET, 1);
    _asm_.out(RESET, 0);
    _asm_.out(READ, 0xAA);
    if (_asm_.in(READ) != 0xAA) {
        Console::WriteLine("Error in reest sound driver.");

    }
}

void SoundBlasterDriver::Start() {
    Assembly _asm_;
    Console console;

    _asm_.out(MIXERPORT, 0x22);
    _asm_.out(MIXERPORT, 0xCC);
    console.WriteLine("Starting Driver...");
    Reset();
    _asm_.out(MIXERPORT, 0x80);
    _asm_.out(MIXERDATAPORT, 0x02);
}

void SoundBlasterDriver::Disable() {
    Assembly _asm_;
    if (channel_number1 == 1) 
        _asm_.out(0x0A, 5);
    else if (channel_number1 == 5) 
        _asm_.out(0xD4, 5);
}

void SoundBlasterDriver::Test() {
    Assembly _asm_;
    Console::WriteLine("Enabling driver by reseting...");
    Reset();

    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xD1);
    while(_asm_.in(WRITE));
    Console::WriteLine("Starting...");
    /*
    _asm_.out(WRITE, 0xF0);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x10);
    _asm_.out(WRITE, 0x00);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x10);
    _asm_.out(WRITE, 0xFF);
    */

    if (channel_number1 == 1) {
        _asm_.out(0x0C, 1);
        _asm_.out(0x0B, 0x58);
        _asm_.out(0x83, 0x10);
        _asm_.out(0x02, 0x50);
        _asm_.out(0x02, 0x04);
        _asm_.out(0x03, 0xFF);
        _asm_.out(0x0A, 1);
    } else if (channel_number1 == 5) {
        _asm_.out(0xD8, 1);
        _asm_.out(0xD6, 0x58);
        _asm_.out(0x8B, 0x10);
        _asm_.out(0xC4, 0x50);
        _asm_.out(0xC4, 0x04);
        _asm_.out(0xC6, 0xFF);
        _asm_.out(0xC6, 0x0F);
        _asm_.out(0xD4, 1);
    }

    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x40);
    _asm_.out(WRITE, 209);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xC0);
    _asm_.out(WRITE, STEREO);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xFE);
    _asm_.out(WRITE, 0x0F);

    return;
}

void SoundBlasterDriver::Test(int inthz) {
    Assembly _asm_;
    Console::WriteLine("Enabling driver by reseting...");
    Reset();

    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xD1);
    while(_asm_.in(WRITE));
    Console::WriteLine("Starting...");
    /*
    _asm_.out(WRITE, 0xF0);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x10);
    _asm_.out(WRITE, 0x00);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x10);
    _asm_.out(WRITE, 0xFF);
    */

    if (channel_number1 == 1) {
        _asm_.out(0x0A, 0x04+1);
        _asm_.out(0x0C, 1);
        _asm_.out(0x0B, 0x58+1);
        _asm_.out(0x83, 0x10);
        _asm_.out(0x02, 0x50);
        _asm_.out(0x02, 0x04);
        _asm_.out(0x03, 0xFF);
        _asm_.out(0x0A, 1);
    } else if (channel_number1 == 5) {
        _asm_.out(0xD4, 0x04+1);
        _asm_.out(0xD8, 1);
        _asm_.out(0xD6, 0x58);
        _asm_.out(0x8B, 0x10);
        _asm_.out(0xC4, 0x50);
        _asm_.out(0xC4, 0x04);
        _asm_.out(0xC6, 0xFF);
        _asm_.out(0xC6, 0x0F);
        _asm_.out(0xD4, 1);
    }

    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0x40);
    _asm_.out(WRITE, inthz);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xC0);
    _asm_.out(WRITE, STEREO);
    while(_asm_.in(WRITE));
    _asm_.out(WRITE, 0xFE);
    _asm_.out(WRITE, 0x0F);

    return;
}
