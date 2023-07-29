#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"

struct gdt_entry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
}__attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

class GDT {
    public:
        static void Init();
};

#endif