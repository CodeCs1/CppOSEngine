#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "types.h"
#include "GDT.h"

class kernel {
    public:
        virtual void BeforeStart();
        virtual void Start();
        virtual void AfterStart();
};

class Assembly {
    public:
        static uint8 in(uint16 _port);
        static void out(uint16 _port, uint8 out);
        static uint16 inw(uint16 _port);
        static void outw(uint16 _port, uint16 out);
        static void cpuid(int code, uint32 *eax, uint32 *ebx, uint32 *ecx, uint32 *edx);
        static void writeslow(uint16 _port, uint8 out);
        static void io_wait();
        static bool is_interrupt_enable();
        
};

#pragma region Old Interrupt

typedef struct {
    uint16 low;
    uint16 kernel;
    uint8 reserved;
    uint8 attributes;
    uint16 high;
}__attribute__((packed)) idt_entry_32;

typedef struct {
    uint16 limit;
    uint32 base;
}__attribute__((packed)) idt_table_32;

#endif