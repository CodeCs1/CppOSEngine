#ifndef __CMOS_H__
#define __CMOS_H__

#define CMOS_A 0x70
#define CMOS_D 0x71

#include "types.h"

struct timevalue {
	unsigned int tv_sec;
	unsigned int tv_usec;
};

class CMOS {
    public:
        static void ReadCMOS(uint16 array[]);
        static void WriteCMOS(uint16 array[]);
        
        class DateTime {

            public:
                void GetDate(uint16* month, uint16* day);
                void GetTime(uint16* hours, uint16* minutes, uint16* seconds);

                static uint32 Now();

                static uint32 GetSeconds();
                static uint32 GetMinute();
                static uint32 GetHour();
                static uint32 GetDay();
                static uint32 GetMonth();
                static uint32 GetYears();
        };
};

#endif