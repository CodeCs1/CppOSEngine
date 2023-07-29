#include "include/PIC.h"
#include "include/kernel.h"

void PIT::PIC_send_EOI(uint8_t irq) {
    if (irq >= 8) {
        Assembly::out(SlaveCommand, EOI);
    }
}

void PIT::Remap(int offset1, int offset2) {
	uint8_t a1, a2;
 
	a1 = Assembly::in(MasterData);                        // save masks
	a2 = Assembly::in(SlaveData);
 
	Assembly::out(MasterCommand, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	Assembly::io_wait();
	Assembly::out(SlaveCommand, ICW1_INIT | ICW1_ICW4);
	Assembly::io_wait();
	Assembly::out(MasterData, offset1);                 // ICW2: Master PIC vector offset
	Assembly::io_wait();
	Assembly::out(SlaveData, offset2);                 // ICW2: Slave PIC vector offset
	Assembly::io_wait();
	Assembly::out(MasterData, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	Assembly::io_wait();
	Assembly::out(SlaveData, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	Assembly::io_wait();
 
	Assembly::out(MasterData, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	Assembly::io_wait();
	Assembly::out(SlaveData, ICW4_8086);
	Assembly::io_wait();
 
	Assembly::out(MasterData, a1);   // restore saved masks.
	Assembly::out(SlaveData, a2);
}

void PIT::IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = MasterData;
    } else {
        port = SlaveData;
        IRQline -= 8;
    }
    value = Assembly::in(port) | (1 << IRQline);
    Assembly::out(port, value);        
}
 
void PIT::IRQ_clear_mask(unsigned char IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = MasterData;
    } else {
        port = SlaveData;
        IRQline -= 8;
    }
    value = Assembly::in(port) & ~(1 << IRQline);
    Assembly::out(port, value);        
}
uint16_t PIT::__pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    Assembly::out(MasterCommand, ocw3);
    Assembly::out(SlaveCommand, ocw3);
    return (Assembly::in(SlaveCommand) << 8) | Assembly::in(MasterCommand);
}

uint16_t PIT::pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}
 
/* Returns the combined value of the cascaded PICs in-service register */
uint16_t PIT::pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}