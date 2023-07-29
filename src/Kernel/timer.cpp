#include "include/timer.h"
#include "include/IDT.h"
#include "include/console.h"
#include "include/kernel.h"

void timer_phase(int hz)
{
    Assembly _asm_;
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    _asm_.out(0x43, 0x36);             /* Set our command byte 0x36 */
    _asm_.out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    _asm_.out(0x40, divisor >> 8);     /* Set high byte of divisor */
}

int timer_ticks = 0;

extern "C" void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 100 == 0)
    {
        Console::WriteLine("One second has passed\n");
    }
}

void Timer::Init() {
    InterruptRequiests::Init_Custom_Handler(0, timer_handler);
}
void Timer::Sleep(int milisecond)
{
    unsigned long eticks;

    eticks = timer_ticks + milisecond;
    while(timer_ticks < eticks);
}