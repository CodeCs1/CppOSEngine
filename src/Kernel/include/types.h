#ifndef __TYPES_H__
#define __TYPES_H__

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int int64;
typedef unsigned long int uint64;
typedef unsigned long int uintptr;
typedef long unsigned int size;

typedef char* string ;
typedef const char* string2;
typedef int8 int8_t;
typedef uint8 uint8_t;
typedef int16 int16_t;
typedef uint16 uint16_t;
typedef int32 int32_t;
typedef uint32 uint32_t;
typedef int64 int64_t;
typedef uint64 uint64_t;
//typedef size size_t;

#define low_16(address) (uint16)((address) & 0xFFFF)
#define high_16(address) (uint16)(((address) >> 16) & 0xFFFF)

#endif