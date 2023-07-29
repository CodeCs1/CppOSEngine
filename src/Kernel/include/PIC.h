#ifndef __PIC_H__
#define __PIC_H__

#include "types.h"
#define MasterCommand 0x0020
#define MasterData    0x0021
#define SlaveCommand  0x00A0
#define SlaveData     0x00A1

#define EOI           0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10

#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */


class PIT {
    public:
        static void PIC_send_EOI(uint8_t irq);
        static void Remap(int offset1, int offset2);
        static void IRQ_set_mask(uint8_t IRQLine);
        static void IRQ_clear_mask(uint8_t IRQline);
        static uint16_t __pic_get_irq_reg(int ocw3);
        static uint16_t pic_get_isr(void);
        static uint16_t pic_get_irr(void);
};

#endif