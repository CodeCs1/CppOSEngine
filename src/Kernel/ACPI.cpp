#include "include/ACPI.h"
#include "include/kernel.h"
#include "include/types.h"

void ACPI::Shutdown() {
    Assembly _asm_;
    _asm_.outw(0x4004, 0x3400);
    _asm_.outw(0x604, 0x2000);
    _asm_.outw(0xB004, 0x2000);
}

void ACPI::Reboot() {
    Assembly _asm_;
    uint8 good=0x02;
    while(good & 0x02)
        good=_asm_.in(0x64);
    _asm_.out(0x64, 0xFE);
    asm("hlt");
}