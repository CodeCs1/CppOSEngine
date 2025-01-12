#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"

class Utils {
    public:
        static void Sleep(int milisecond);
        void * malloc(int nbytes);
        void memory_copy(char *source, char *dest, int nbytes);
        void memory_set(uint8 *dest, uint8 val, uint32 len);
        void memory_copy(string2 source, char* dest, int nbytes);
        void memory_copy(uint8* source, uint8* dest, int nbytes);
        static void* MemorySet(void* s, int c, size n);
        static void * memcpy(void * dest, const void * src, size count);

};

#endif