#include "include/kernel.h"
#include "include/console.h"
#include "include/types.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void kernel::BeforeStart() {
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

void Interrupt::set_idt_gate(int n, uint32 handler) {
    idt[n].low_offset=low_16(handler);
    idt[n].sel=KERNEL_CS;
    idt[n].always0=0;
    idt[n].flags=0x8E;
    idt[n].high_offset=high_16(handler);
}

void Interrupt::set_ldt() {
    idt_reg.base = 0;
    idt_reg.limit = IDT_ENTRIES*sizeof(idt_gate_t) -1;
    __asm__ __volatile__("lidtl (%0)" : :"r" (&idt_reg));
}

void Interrupt::isr_install() {
    Exception ex;
    set_idt_gate(0, isr0());
    set_idt_gate(1, isr1());
    set_idt_gate(2, isr2());
    set_idt_gate(3, isr3());
    set_idt_gate(4, isr4());
    set_idt_gate(5, isr5());
    set_ldt();
}

Exception ex;

uint32 isr0() {
    ex.Divbyzero();
    return 0;
}
uint32 isr1() {
    ex.InvaildOpcode();
    return 0;
}

uint32 isr2() {
    ex.OutOfBounds();
    return 0;
}
uint32 isr3() {
    ex.PageFault();
    return 0;
}
uint32 isr4() {
    ex.StackFault();
    return 0;
}
uint32 isr5() {
    ex.Unknown();
    return 0;
}

string2 exception_messages[] = {
    "Division By Zero (R/0 is not allowed)",
    "Out of Bounds",
    "Invalid Opcode",
    "Stack Fault",
    "Page Fault",
    "Unknown Interrupt"
};

Console c;

void Exception::Divbyzero() {
    c.WriteLine(exception_messages[0]);
}
void Exception::InvaildOpcode() {
    c.WriteLine(exception_messages[1]);
    asm("hlt");
}
void Exception::OutOfBounds() {
    c.WriteLine(exception_messages[2]);
    asm("hlt");
}
void Exception::Unknown() {
    c.WriteLine(exception_messages[3]);
    asm("hlt");
}
void Exception::PageFault() {
    c.WriteLine(exception_messages[4]);
    asm("hlt");
}
void Exception::StackFault() {
    c.WriteLine(exception_messages[5]);
    asm("hlt");
}