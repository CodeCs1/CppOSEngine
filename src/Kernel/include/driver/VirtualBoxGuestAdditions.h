#ifndef __VirtualBoxGuestAdditions_H__
#define __VirtualBoxGuestAdditions_H__

#include "../types.h"
#include <stdint.h>

//Testing
class VirtualBoxGuestAdditions {
    // Allocate one page of memory, providing its physical address as an output and virtual address as a return value.
    void * allocate_physical_page(uint32_t * physical);
    // Map a physical page into the virtual memory space.
    void * map_physical_page(uint32_t physical);
};

#endif