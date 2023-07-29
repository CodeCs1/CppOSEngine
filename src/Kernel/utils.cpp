#include "include/utils.h"
#include "include/console.h"
#include "include/kernel.h"
#include "include/types.h"
#include "include/string.h"

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

const char* Utils::malloc2(size size) {

    last_alloc = end + 0x1000;
    heap_begin = last_alloc;
    pheap_end = 0x400000;
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
	heap_end = pheap_begin;
    memset((uint8*)heap_begin, 0, heap_end - heap_begin);

    if(!size) return 0;
    uint8_t* mem = (uint8_t*)heap_begin;
    while((uint64_t)mem < last_alloc) {
        alloc_t* a = (alloc_t*)mem;
        if(!a->size)
            goto malloc1;
        if(a->status) {
            mem += a->size;
            mem += sizeof(alloc_t);
            mem += 4;
            continue;
        }

        if (a->size >=size) {
            a->status = 1;
            memset(mem+sizeof(alloc_t), 0, size);
            memory_used += size + sizeof(alloc_t);
            return (char*)(mem+sizeof(alloc_t));
        }

        mem+=a->size;
        mem+=sizeof(alloc_t);
        mem+=4;
    }

    malloc1:;
    if(last_alloc+size+sizeof(alloc_t)  >= heap_end) {
        Console::WriteLine2("Can't allocate!");
        Console::WriteLine2(itos(last_alloc+size+sizeof(alloc_t)));
        __asm__("cli; hlt");
    }
    alloc_t* alloc = (alloc_t*)last_alloc;
    alloc->status = 1;
    alloc->size = size;

    last_alloc += size;
    last_alloc += sizeof(alloc_t);
    last_alloc +=4;
    memory_used += size+4+sizeof(alloc_t);
    memset((uint8_t*)alloc+sizeof(alloc_t), 0, size);
    Console::WriteLine2(string2(100213));
    return string2((uint8_t*)alloc + sizeof(alloc_t));
}

void * Utils::memcpy(void * dest, const void * src, size count) {
	asm volatile ("cld; rep movsb" : "+c" (count), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}