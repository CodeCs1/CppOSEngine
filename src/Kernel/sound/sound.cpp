#include "../include/Sound/sound.h"
#include "../include/types.h"
#include "../include/kernel.h"
#include "../include/utils.h"
#include "../include/console.h"

#define PC_SPEAKER 0x61

void play_sound(uint32 nFrequence) {
    uint32 div;
    uint8 tmp;
    Assembly _asm_;


    div = 1193180 / nFrequence;
    _asm_.out(0x43, 0xb6);
 	_asm_.out(0x42, (uint8) (div) );
 	_asm_.out(0x42, (uint8) (div >> 8));
}

void Sound::EnableSound() {
    Assembly _asm_;
    uint8 tmp = _asm_.in(PC_SPEAKER);
    _asm_.out(PC_SPEAKER,tmp|3);
}

void Sound::Stop() {
    Assembly _asm_;
    Console console;

    uint8 tmp = _asm_.in(PC_SPEAKER);
    _asm_.out(PC_SPEAKER, tmp & 0xFC);
}

void Sound::BeepTest() {
    Assembly _asm_;
    Utils utils;
    EnableSound();

    play_sound(C1);
    utils.Sleep(333);
    play_sound(C2);
    utils.Sleep(333);
    play_sound(C3);
    utils.Sleep(333);
    play_sound(C4);
    utils.Sleep(333);
    play_sound(C5);
    utils.Sleep(333);
    play_sound(C6);
    utils.Sleep(333);
    play_sound(C7);
    utils.Sleep(333);
    play_sound(C8);
    utils.Sleep(333);

    Stop();

    Console console;
    console.WriteLine("<Testing completed>");
}