#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"
#include "console.h"

uint8 strEql(string2 ch1, string2 ch2);

void* int_to_ascii(int n, char str[]);
uint16 stringlen(string2 ch);

string itos(int n);

int stoi(string ch);

int stoi(string2 ch);

void* memset(void* dest, uint8 value, uint32 size);
void memcpy(void* dest, void* src, uint32 size);

#endif