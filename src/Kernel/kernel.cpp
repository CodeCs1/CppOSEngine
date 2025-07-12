#include "include/kernel.h"
#include "include/console.h"
#include "include/types.h"
#include "include/multiboot.h"
#include "include/IDT.h"
#include "pmm.h"
#include <driver/Keyboard.h>

#define MASTERCOMMAND 0x20
#define MASTERDATA 0x21
#define SLAVECOMMAND 0xA0
#define SLAVEDATA 0xA1

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


extern "C" void main_kernel(uint32_t start, uint32_t end,uint64_t magic, Multiboot_info* info) {
  Interrupt::idt_install();
  InterruptRequiests::Init();
  InterruptServiceRoutines::Init();
  Keyboard::Init();
  Console::Init();

  PMM::Init(end-start, 0x100000);

  run_kernel();
}

