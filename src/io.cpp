#include <stdio.h>

#include "io.h"

static read_file_32
ReadFile32(const wchar_t *path){
    read_file_32 result;
    ZeroStruct(&result);
    
    FILE *in = _wfopen(path, L"rb");
    if(in){
        if(fseek(in, 0, SEEK_END) == 0){
            
            u32 size = ftell (in);
            u8 *contents = (u8 *) malloc(sizeof(u8)*size);
            if(contents){
                if(fseek(in, 0, SEEK_SET) == 0){
                    size_t readBytes = fread(contents, sizeof(u8), (size_t) size, in);
                    if(readBytes == (size_t) size){
                        result.contents = contents;
                        result.size     = size;
                    } else {
                        result.err = 1;
                    }
                } else{
                    result.err = 2;
                }
                
                if(result.err){
                    mfree((void **)&contents);
                }
            } else {
                result.err = 3;
            }
        } else {
            result.err = 4;
        }
        
        fclose(in);
    } else {
        result.err = 5;
    }
    
    return result;
}

static write_file_32 
WriteFile32(const wchar_t *path, u8 *bytes, u32 size){
    write_file_32 result;
    ZeroStruct(&result);
    
    FILE *out = _wfopen(path, L"wb");
    if(out){
        size_t wBytes = fwrite(bytes, sizeof(u8), (size_t)size, out);
        if(wBytes == (size_t)size){
            result.err = 0;
        } else {
            result.err = 1;
        }
        fclose(out);
    } else {
        result.err = 2;
    }
    
    return result;
}