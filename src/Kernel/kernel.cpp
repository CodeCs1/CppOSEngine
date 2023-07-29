#include "include/kernel.h"
#include "include/ACPI.h"
#include "include/GDT.h"
#include "include/Sound/soundblaster.h"
#include "include/console.h"
#include "include/types.h"

#define MASTERCOMMAND 0x20
#define MASTERDATA 0x21
#define SLAVECOMMAND 0xA0
#define SLAVEDATA 0xA1

void kernel::BeforeStart() {
    Console console;
    console.WriteLine("Hello World!");
    Start();
}

void kernel::Start() {

}
void kernel::AfterStart() {
    __asm__ __volatile__("hlt");
}

//in{b8bit,w16bit}

uint8 Assembly::in(uint16 _port) {
    uint8 rv;
    __asm__ __volatile__("inb %1, %0": "=a" (rv) : "Nd" (_port));
    return rv;
}

void Assembly::out(uint16 _port, uint8 out) {
    __asm__ __volatile__("outb %0, %1" : :"a" (out), "Nd" (_port));
}

uint16 Assembly::inw(uint16 _port ) {
    uint16 rv;
    __asm__ __volatile__("inw %1, %0" : "=a" (rv) : "Nd" (_port));
    return rv;
}

void Assembly::outw(uint16 _port, uint16 out) {
     __asm__ volatile("outw %0, %1" : : "a" (out), "Nd" (_port));
}

void Assembly::writeslow(uint16 _port, uint8 out) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (out), "Nd" (_port));
}

void Assembly::io_wait() {
    out(0x80, 0);
}

bool Assembly::is_interrupt_enable() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

void Assembly::cpuid(int code, uint32 *eax, uint32 *ebx, uint32* ecx, uint32 *edx) {
    __asm__ volatile("cpuid": "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (code));
}