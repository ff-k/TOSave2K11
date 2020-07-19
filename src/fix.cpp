#include <wchar.h>

#include "util.h"
#include "io.cpp"

#include "crc32.cpp"

typedef struct {
    u32 err;
} fix_result;

static fix_result
FixSaveFile(u8 *sfContents, u32 sfSize, const wchar_t *mfPath){
    fix_result result;
    ZeroStruct(&result);
    
    const u32 zeroAddr[] = {
        0x5F2458, 0x5F2459, 0x5F245A, 0x5F245B, 
        0x6861FC, 0x6861FD, 0x6861FE, 0x6861FF, 
    };
    
    for(u32 i=0; i<StaticArraySize(zeroAddr); i++){
        sfContents[zeroAddr[i]] = 0;
    }
    
    sfContents[0x6861E8] = sfContents[0x5F2444] = 9;
    sfContents[0x68622C] = sfContents[0x5F2488] = 1;
    
    crc32b_init();
    
    unsigned int crc = crc32b_table_reflect( sfSize-4, sfContents+4 );
    
    sfContents[0] = (crc >> 0) & 0xFF;
    sfContents[1] = (crc >> 8) & 0xFF;
    sfContents[2] = (crc >>16) & 0xFF;
    sfContents[3] = (crc >>24) & 0xFF;
    
    write_file_32 wf = WriteFile32(mfPath, sfContents, sfSize);
    if(wf.err){
        result.err = 1;
    }
    
    return result;
}

static fix_result
FixSaveFile(const wchar_t *sfPath, const wchar_t *mfPath){
    fix_result result;
    ZeroStruct(&result);
    
    read_file_32 rf = ReadFile32(sfPath);
    if(rf.err == 0){
        result = FixSaveFile(rf.contents, rf.size, mfPath);
    } else {
        result.err = 2;
    }
    
    return result;
}
