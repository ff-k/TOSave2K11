#ifndef INC_UTIL_H
#define INC_UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define StaticArraySize(arr)             \
    (sizeof( (arr) )/sizeof( (arr)[0] ))

#define ZeroStruct(str)                  \
    do {                                 \
        memset((str), 0, sizeof(*str) ); \
    } while(0);

typedef uint32_t u32;
typedef uint8_t  u8;

static void
mfree(void **ptr){
    free(*ptr);
    *ptr = 0;
}

#endif