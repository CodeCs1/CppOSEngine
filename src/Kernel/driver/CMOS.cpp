#include "../include/CMOS.h"
#include "../include/Core.h"

//code based on https://github.com/stevej/osdev/blob/9b745b5de0ba198d3ba245de62453012f346d5af/kernel/devices/cmos.c

#define from_bcd(val) (val & 0x0F) + ((val / 16) * 10)

void Init() {
    Assembly _asm_;
    _asm_.out(0x70, 0x8A);
    _asm_.out(0x71, 0x20);

    _asm_.out(0x70, 0x8B);		// select register B, and disable NMI
    char prev=_asm_.in(0x71);	// read the current value of register B
    _asm_.out(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
    _asm_.out(0x71, prev | 0x40);
    _asm_.out(0x70, 0x8A);		// set index to register A, disable NMI
    _asm_.out(0x70, 0x0C);	// select register C
    _asm_.in(0x71);
}

void CMOS::ReadCMOS(uint16 *array) {
    Assembly _asm_;
    Init();
    for(uint16_t index =0;index<128;++index) {
        _asm_.out(0x70, index);
        array[index] = _asm_.in(0x71);
    }
}

void CMOS::WriteCMOS(uint16 *array) {
    uint8 index;
    Assembly _asm_;
    for(index=0;index<128;++index) {
        _asm_.out(0x70, index);
        _asm_.out(0x71, index);
    }
}

void CMOS::DateTime::GetDate(uint16* month, uint16* day) {
    uint16_t values[128];
    ReadCMOS(values);

    *month = from_bcd(values[8]);
    *day = from_bcd(values[7]);
}

void CMOS::DateTime::GetTime(uint16 *hours, uint16 *minutes, uint16 *seconds) {
    uint16_t values[128]; /* CMOS dump */
	ReadCMOS(values);

	*hours   = from_bcd(values[4]);
	*minutes = from_bcd(values[2]);
	*seconds = from_bcd(values[0]);
}

uint32 secs_of_month(int months, int years) {
    years += 2000;

    uint32_t days = 0;
    switch(months) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            days += 31;
        
        case 4:
        case 6:
        case 9:
        case 11:
            days += 30;

        case 2:
            days += 28;
            if ((years % 4 == 0) && (years % 100 != 0) || (years % 400 ==0)) {
                days++;
            }
        default:
            break;
    }
    return days * 86400;
}


uint32 secs_of_years(int years) {
    uint32_t days = 0;
    years += 2000;
    while(years > 1969) {
        days += 365;
        if (years % 4 == 0) {
			if (years % 100 == 0) {
				if (years % 400 == 0) {
					days++;
				}
			} else {
				days++;
			}
		}
		years--;
    }
    return days * 86400;
}

int gettimeofday(struct timevalue * t, void *z) {
	uint16 values[128];
	CMOS::ReadCMOS(values);

	uint32 time = secs_of_years(from_bcd(values[9]) - 1) +
					secs_of_month(from_bcd(values[8]) - 1, from_bcd(values[9])) + 
					(from_bcd(values[7]) - 1) * 86400 +
					(from_bcd(values[4])) * 3600 +
					(from_bcd(values[2])) * 60 +
					from_bcd(values[0]) +
					0;
	t->tv_sec = time;
	t->tv_usec = 0;

	return 0;
}

uint32 CMOS::DateTime::Now() {
    struct timevalue t;
    gettimeofday(&t, __null);
    return t.tv_sec;
}

uint32 CMOS::DateTime::GetSeconds() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 secounds = from_bcd(values[0])+0;
    return secounds;
}
uint32 CMOS::DateTime::GetMinute() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 minutes = from_bcd(values[2])*1;
    return minutes;
}
uint32 CMOS::DateTime::GetHour() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 hour = from_bcd(values[4])*1;
    return hour;
}
uint32 CMOS::DateTime::GetDay() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 day = from_bcd(values[7]-1)*1;
    return day;
}

uint32 CMOS::DateTime::GetMonth() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 month = secs_of_month(from_bcd(values[8]) - 1, from_bcd(values[9]));
    return month;
}
uint32 CMOS::DateTime::GetYears() {
    uint16 values[128];
    CMOS::ReadCMOS(values);
    uint32 years = secs_of_years(from_bcd(values[9]) - 1);
    return years;
}