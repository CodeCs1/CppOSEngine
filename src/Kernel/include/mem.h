#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"


void* memset(void* ptr, uint32_t value, size sz);
void* memsetb(uint8_t* addr, uint8_t byte, uint64_t sz);
void* memsetw(uint16_t* addr, uint8_t byte, uint64_t sz);

#endif
