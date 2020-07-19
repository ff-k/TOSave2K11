#ifndef INC_IO_H
#define INC_IO_H

#include "util.h"

typedef struct {
    u32 err;
    
    u8 *contents;
    u32 size;
} read_file_32;

typedef struct {
    u32 err;
} write_file_32;

#endif