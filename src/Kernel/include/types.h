#ifndef __TYPES_H__
#define __TYPES_H__

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int int64;
typedef unsigned long long int uint64;

typedef char* string ;
typedef const char* string2;

#define low_16(address) (uint16)((address) & 0xFFFF)
#define high_16(address) (uint16)(((address) >> 16) & 0xFFFF)

#endif