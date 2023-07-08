#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "types.h"

class kernel {
    public:
        virtual void BeforeStart();
        virtual void Start();
        virtual void AfterStart();
};

class Assembly {
    public:
        uint8 in(uint16 _port);
        void out(uint16 _port, uint8 out);

        uint16 inw(uint16 _port);
        void outw(uint16 _port, uint16 out);
};

typedef struct {
    uint16 low_offset;
    uint16 sel;
    uint8 always0;
    uint8 flags;
    uint16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16 limit;
    uint32 base;
} __attribute__((packed)) idt_register_t;

class Interrupt {
    public:
        void set_idt_gate(int n, uint32 handler);
        void set_ldt();
        
        void isr_install();
};

#define IDT_ENTRIES 256
#define KERNEL_CS 0x08

uint32 isr0();
uint32 isr1();
uint32 isr2();
uint32 isr3();
uint32 isr4();
uint32 isr5();

class Exception {
    public:
        void Divbyzero();
        void OutOfBounds();
        void InvaildOpcode();
        void StackFault();
        void PageFault();
        void Unknown();
};


#endif