#include "include/utils.h"
#include "include/types.h"
typedef struct {
	uint8_t status;
	uint32_t size;
} alloc_t;

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
void* Utils::malloc(int nbytes)
{
	char variable[nbytes];
	return &variable[nbytes];
}

void Utils::memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}
void Utils::memory_copy(uint8* source, uint8 *dest, int nbytes) {
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

void* Utils::MemorySet(void *s, int c, size n) {
    int i;
    for(i=0;i<n;i++) {
        ((char*)s)[i]=c;
    }
    return s;
}

void* memset(uint8* ptr, int value, size num) {
    unsigned char* p=ptr;
    while(num--) {
        *p++ = (uint8)value;
    }
    return ptr;
}

#define MAX_PAGE_ALIGNED_ALLOCS 32

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;

extern "C" uint32_t end;
extern "C" uint32_t base;	

void * Utils::memcpy(void * dest, const void * src, size count) {
	asm volatile ("cld; rep movsb" : "+c" (count), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}