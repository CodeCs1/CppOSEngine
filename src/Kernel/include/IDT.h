#ifndef __IDT_H__
#define __IDT_H__

#include "types.h"

struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

class Interrupt{
    public:
        static void idt_install(void);
};

typedef void (*irq_handler_t) (struct regs *);

//or ISR.
class InterruptServiceRoutines {
    public:
        static void Init(void);
};

class InterruptRequiests {
    public:
        static void Init(void);
        static void Init_Custom_Handler(uint8_t irq, irq_handler_t handler);
        static void Uninstall_Hndler(int irq);
};


#endif