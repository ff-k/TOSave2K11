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

typedef struct {
    u32 Val[3];
} u32_triplet;

static void
mfree(void **ptr){
    free(*ptr);
    *ptr = 0;
}

static int 
NumberOfBitsSet(u32 Val){
    int result = 0;
    
    u32 cursor = 0x1;
    for(int i=0; i<32; i++){
        if(Val & cursor){
            result++;
        }
        cursor = cursor << 1;
    }
    
    return result;
}

#endif