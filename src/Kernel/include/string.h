#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"
#include "console.h"

uint8 strEql(string2 ch1, string2 ch2);

void* int_to_ascii(int n, char str[]);

string itos(int n);

int stoi(string ch);

int stoi(string2 ch);


#endif