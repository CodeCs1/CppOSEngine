#include <pmm.h>
#include <mem.h>

uint32_t mem_size = 0;
uint32_t mem_used = 0;
uint32_t* mem_map = nullptr;

void PMM_set(int bit) {
    mem_map[bit/32] |= (1 << (bit % 32));
}
void PMM_clear(int bit) {
    mem_map[bit/32] &= ~(1 << (bit % 32));
}
bool PMM_test(int bit) {
    return (mem_map[bit/32] & (1 << (bit % 32)));
}

int PPM_first_free() {
    for (uint32_t i = 0; i < mem_size; i++) {
        if (mem_map[i] != 0xffffffff) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(mem_map[i] & (1 << j))) {
                    return i *4*BLOCK_PER_BYTE + j;
                }
            }
        }
    }
    return -1;
}

void PMM::Init(uint32_t memsize, uint32_t bitmap_addr) {
  // Initialize the memory manager with the given size
  // This function should set up the memory pool and free list

  mem_map = (uint32_t*)bitmap_addr;
  mem_size = memsize;
  memset(mem_map, 0xff, mem_size / BLOCK_PER_BYTE);

}
void* PMM::Alloc(uint32_t size) {
  // Allocate a block of memory of the given size
  // This function should find a free block and return its address
  if (GetFreeMemory() <= size) return 0;
  int frame = PPM_first_free();
    if (frame == -1) {
        return nullptr; // No free memory available
    }
    PMM_set(frame);
    uint32_t addr= frame * BLOCK_SIZE;
    mem_used++;
  return (void*)addr;
}
void PMM::Free(void* ptr) {
  // Free a previously allocated block of memory
  // This function should mark the block as free in the memory pool
}

uint32_t PMM::GetFreeMemory() {
  // Return the amount of free memory available
  return mem_size - mem_used;
}