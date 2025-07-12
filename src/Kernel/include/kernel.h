#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "types.h"


#define CALL_MAIN_KERNEL(name) void run_kernel() {\
	name m; \
	kernel* k = &m; \
	k->BeforeStart(); \
	k->Start(); \
}


class kernel {
    public:
        virtual void BeforeStart();
        virtual void Start();
};

void run_kernel();

class Assembly {
    public:
        static uint8 in(uint16 _port);
        static void out(uint16 _port, uint8 out);
        static uint16 inw(uint16 _port);
        static void outw(uint16 _port, uint16 out);
        static void writeslow(uint16 _port, uint8 out);
        static void io_wait();
        
};

#endif
