#include "include/mem.h"

#include "include/types.h"
/*
void Sleep(int milisecond) {
    for (int i = 0; i < milisecond * 100000; i++)
    {
        ;
        ;
        ;
        ;
        ;
    }
}
*/

void* memset(void* ptr, uint32_t value, size num) {
	uint8_t* p = (uint8_t*)ptr;
	while(num--) *p++ = value;

	return ptr;
}

void* memsetb(uint8_t* ptr, uint8_t value, size num) {
    unsigned char* p=ptr;
    while(num--) {
        *p++ = value;
    }
    return ptr;
}


void* memsetw(uint16_t* ptr, uint16_t value, size num) {
    uint16_t* p=ptr;
    while(num--) {
        *p++ = value;
    }
    return ptr;
}
