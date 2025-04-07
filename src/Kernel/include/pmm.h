#pragma once

#include <types.h>

#define BLOCK_PER_BYTE 8
#define BLOCK_SIZE 4096
#define BLOCK_ALIGN BLOCK_SIZE

class PMM {
    public:
        static void Init(uint32_t mem_size, uint32_t bitmap);
        static void* Alloc(uint32_t size);
        static void Free(void* ptr);
        static void* AllocAligned(uint32_t size, uint32_t align);
        static void FreeAligned(void* ptr);
        static uint32_t GetFreeMemory();
        static uint32_t GetUsedMemory();
        static uint32_t GetTotalMemory();
};