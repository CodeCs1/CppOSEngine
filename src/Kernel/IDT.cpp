#include "include/IDT.h"
#include "include/ACPI.h"
#include "include/console.h"
#include "include/driver/Keyboard.h"
#include "include/kernel.h"
#include "include/mem.h"
#include "include/types.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

struct regs {
  unsigned int gs, fs, es, ds; /* pushed the segs last */
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
  unsigned int int_no, err_code; /* our 'push byte #' and ecodes do this */
  unsigned int eip, cs, eflags, useresp,
      ss; /* pushed by the processor automatically */
};

const char *exception_messages[] = {"Division By Zero",
                                    "Debug",
                                    "Non Maskable Interrupt",
                                    "Breakpoint",
                                    "Into Detected Overflow",
                                    "Out of Bounds",
                                    "Invalid Opcode",
                                    "No Coprocessor",

                                    "Double Fault",
                                    "Coprocessor Segment Overrun",
                                    "Bad TSS",
                                    "Segment Not Present",
                                    "Stack Fault",
                                    "General Protection Fault",
                                    "Page Fault",
                                    "Unknown Interrupt",

                                    "Coprocessor Fault",
                                    "Alignment Check",
                                    "Machine Check",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",

                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved"};
extern "C" void idt_load();
extern "C" void _isr0();
extern "C" void _isr1();
extern "C" void _isr2();
extern "C" void _isr3();
extern "C" void _isr4();
extern "C" void _isr5();
extern "C" void _isr6();
extern "C" void _isr7();
extern "C" void _isr8();
extern "C" void _isr9();
extern "C" void _isr10();
extern "C" void _isr11();
extern "C" void _isr12();
extern "C" void _isr13();
extern "C" void _isr14();
extern "C" void _isr15();
extern "C" void _isr16();
extern "C" void _isr17();
extern "C" void _isr18();
extern "C" void _isr19();
extern "C" void _isr20();
extern "C" void _isr21();
extern "C" void _isr22();
extern "C" void _isr23();
extern "C" void _isr24();
extern "C" void _isr25();
extern "C" void _isr26();
extern "C" void _isr27();
extern "C" void _isr28();
extern "C" void _isr29();
extern "C" void _isr30();
extern "C" void _isr31();

extern "C" void _irq0();
extern "C" void _irq1();
extern "C" void _irq2();
extern "C" void _irq3();
extern "C" void _irq4();
extern "C" void _irq5();
extern "C" void _irq6();
extern "C" void _irq7();
extern "C" void _irq8();
extern "C" void _irq9();
extern "C" void _irq10();
extern "C" void _irq11();
extern "C" void _irq12();
extern "C" void _irq13();
extern "C" void _irq14();
extern "C" void _irq15();

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

void Interrupt::idt_install() {
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = (uintptr)&idt;
  //memset(idt, 0, sizeof(struct idt_entry) * 256);
  memset(idt, 0, sizeof(struct idt_entry)*256);
  idt_load();
}

extern "C" void fault_handler(struct regs *r) {
  if (r->int_no < 32) {
    Console::ForegroundColor(ConsoleColor::White);
    Console::Write("Exception occurred while running Kernel!\nPlease check "
                   "your code and then try again!\n");
    Console::Write("Error Message: ");
    Console::ForegroundColor(ConsoleColor::Red);
    Console::WriteLine(exception_messages[r->int_no]);
    Console::ForegroundColor(ConsoleColor::White);
    Console::Write("Error Code: ");
    Console::ForegroundColor(ConsoleColor::Red);
    Console::Write("0x0");
    Console::WriteLine(itos(r->int_no));
    asm("cli; hlt");
  }
}

void InterruptServiceRoutines::Init() {
  idt_set_gate(0, (uint64_t)_isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint64_t)_isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint64_t)_isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint64_t)_isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint64_t)_isr4, 0x08, 0x8E);
  idt_set_gate(5, (uint64_t)_isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint64_t)_isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint64_t)_isr7, 0x08, 0x8E);

  idt_set_gate(8, (uint64_t)_isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint64_t)_isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint64_t)_isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint64_t)_isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint64_t)_isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint64_t)_isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint64_t)_isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint64_t)_isr15, 0x08, 0x8E);

  idt_set_gate(16, (uint64_t)_isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint64_t)_isr17, 0x08, 0x8E);
  idt_set_gate(18, (uint64_t)_isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint64_t)_isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint64_t)_isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint64_t)_isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint64_t)_isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint64_t)_isr23, 0x08, 0x8E);

  idt_set_gate(24, (uint64_t)_isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint64_t)_isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint64_t)_isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint64_t)_isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint64_t)_isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint64_t)_isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint64_t)_isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint64_t)_isr31, 0x08, 0x8E);
}

static irq_handler_t irq_routines[16] = {__null};

void InterruptRequiests::Init_Custom_Handler(uint8_t irq, irq_handler_t handler) {
  irq_routines[irq] = handler;
}

void InterruptRequiests::Uninstall_Hndler(int irq) { irq_routines[irq] = 0; }

void irq_remap(void) {
  Assembly _asm_;
  _asm_.out(0x20, 0x11);
  _asm_.out(0xA0, 0x11);
  _asm_.out(0x21, 0x20);
  _asm_.out(0xA1, 0x28);
  _asm_.out(0x21, 0x04);
  _asm_.out(0xA1, 0x02);
  _asm_.out(0x21, 0x01);
  _asm_.out(0xA1, 0x01);
  _asm_.out(0x21, 0x0);
  _asm_.out(0xA1, 0x0);
}

void InterruptRequiests::Init() {
  irq_remap();
  idt_set_gate(32, (uint64_t)_irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint64_t)_irq1, 0x08, 0x8E);
  idt_set_gate(34, (uint64_t)_irq2, 0x08, 0x8E);
  idt_set_gate(35, (uint64_t)_irq3, 0x08, 0x8E);
  idt_set_gate(36, (uint64_t)_irq4, 0x08, 0x8E);
  idt_set_gate(37, (uint64_t)_irq5, 0x08, 0x8E);
  idt_set_gate(38, (uint64_t)_irq6, 0x08, 0x8E);
  idt_set_gate(39, (uint64_t)_irq7, 0x08, 0x8E);
  idt_set_gate(40, (uint64_t)_irq8, 0x08, 0x8E);
  idt_set_gate(41, (uint64_t)_irq9, 0x08, 0x8E);
  idt_set_gate(42, (uint64_t)_irq10, 0x08, 0x8E);
  idt_set_gate(43, (uint64_t)_irq11, 0x08, 0x8E);
  idt_set_gate(44, (uint64_t)_irq12, 0x08, 0x8E);
  idt_set_gate(45, (uint64_t)_irq13, 0x08, 0x8E);
  idt_set_gate(46, (uint64_t)_irq14, 0x08, 0x8E);
  idt_set_gate(47, (uint64_t)_irq15, 0x08, 0x8E);
  asm volatile("sti");

}
void irq_ack(int irq_no) {
  Assembly _asm_;
  if (irq_no >= 12) {
    _asm_.out(0xA0, 0x20);
  }
  _asm_.out(0x20, 0x20);
}
extern "C" void irq_handler(struct regs *r) {
  asm volatile("cli");
  void (*handler)(struct regs *r);
  if (r->int_no > 47 || r->int_no < 32) {
    handler = 0;
  } else {
    handler = irq_routines[r->int_no - 32];
  }
  if (handler) {
    handler(r);
  } else {
    irq_ack(r->int_no - 32);
  }
  asm volatile("sti");
}
