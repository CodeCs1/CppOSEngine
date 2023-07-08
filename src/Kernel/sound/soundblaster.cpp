#include "../include/Sound/soundblaster.h"
#include "../include/kernel.h"
#include "../include/utils.h"
#include "../include/console.h"
#include "../include/string.h"

bool sb16Present = false;
uint16 sb16BaseAddress = 0x220;
uint8 sb16VersionMajor;
uint8 sb16VersionMinor;
uint8 sb16IRQ = 5;    // TODO: provide some way to configure the SB16 IRQ
uint8 sb16_8bitDMAchannel = 1; // TODO: Make these configurable as well
uint8 sb16_16bitDMAchannel = 5;

uint8 Read() {
    Assembly _asm_;
    while(_asm_.in(sb16BaseAddress + DSP_WRITE) & READ_WRITE_READY_BIT)
        ;

    return _asm_.in(sb16BaseAddress+DSP_READ);
}

void Write(uint8 data) {
    Assembly _asm_;
    while(_asm_.in(sb16BaseAddress + DSP_WRITE) & READ_WRITE_READY_BIT)
        ;

    _asm_.out(sb16BaseAddress + DSP_WRITE,data);
}

bool SoundBlasterDriver::Reset() {
    Assembly _asm_;
    Utils utils;
    sb16Present=false;
    _asm_.out(sb16BaseAddress+ DSP_RESET, 1);
    utils.Sleep(2);
    _asm_.out(sb16BaseAddress+DSP_RESET, 0);

    utils.Sleep(2);
    if (!(_asm_.in(sb16BaseAddress+DSP_STATUS)&READ_WRITE_READY_BIT)) {
        return false;
    }
    uint8 readVal = _asm_.in(sb16BaseAddress+DSP_READ);
    if (readVal==DSP_READY){
        sb16BaseAddress=true;
    }
    return sb16Present;
}

void SoundBlasterDriver::Enable() {
    sb16BaseAddress = SB16_BASE0;
    Console console;
    if (!Reset()) {
        sb16BaseAddress = SB16_BASE1;
        if (!Reset()) {
            sb16BaseAddress = SB16_BASE2;
            if (!Reset()) {
                console.WriteLine("E");
                return;
            }
        }
    }

    if (!sb16Present) {
        console.WriteLine("[ERROR]: SoundBlaster16 not found!");
        return;
    }

    Write(DSP_CMD_VERSION);
    sb16VersionMajor=Read();
    sb16VersionMinor=Read();

    console.WriteLine("Found SoundBlaster at: ");
    Hex hex;
    hex.printfHex(sb16BaseAddress);

    console.WriteLine("Version:");
    console.WriteLine(itos(sb16VersionMajor));
    console.WriteChr(',');
    console.WriteLine(itos(sb16VersionMinor));
}

void SoundBlasterDriver::Start() {

}