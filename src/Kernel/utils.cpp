#include "include/utils.h"


void Utils::Sleep(int milisecond) {
    for (int i = 0; i < milisecond * 100000; i++)
    {
        ;
        ;
        ;
        ;
        ;
    }
}
void * Utils::malloc(int nbytes)
{
	auto variable = char(nbytes);
	return nullptr;
}

void Utils::memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}
void Utils::memory_copy(string2 source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}
void Utils::memory_set(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}
