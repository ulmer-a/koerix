#pragma once

#define NULL ((void*)0)

/* Integer types */
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

typedef signed long ssize_t;
typedef unsigned long size_t;

/* Attributes */
#define _INIT
#define _NORETURN __attribute__((noreturn))
#define _PACKED __attribute((packed))
#define _PAGE_ALIGN __attribute((aligned(4096)))
#define _SECTION(s) __attribute__((section(s), used))


#define BIT(x) (1ul << (x))