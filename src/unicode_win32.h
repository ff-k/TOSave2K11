#ifndef INC_UNICODE_WIN32_H
#define INC_UNICODE_WIN32_H

#include "util.h"

u32_triplet unicodeSubsetBitfields[] = {
    {   0,    0x000000,    0x00007F}, 
    {   1,    0x000080,    0x0000FF}, 
    {   2,    0x000100,    0x00017F}, 
    {   3,    0x000180,    0x00024F}, 
    {   4,    0x000250,    0x0002AF}, 
    {   4,    0x001D00,    0x001D7F}, 
    {   4,    0x001D80,    0x001DBF}, 
    {   5,    0x0002B0,    0x0002FF}, 
    {   5,    0x00A700,    0x00A71F}, 
    {   6,    0x000300,    0x00036F}, 
    {   6,    0x001DC0,    0x001DFF}, 
    {   7,    0x000370,    0x0003FF}, 
    {   8,    0x002C80,    0x002CFF}, 
    {   9,    0x000400,    0x0004FF}, 
    {   9,    0x000500,    0x00052F}, 
    {   9,    0x002DE0,    0x002DFF}, 
    {   9,    0x00A640,    0x00A69F}, 
    {  10,    0x000530,    0x00058F}, 
    {  11,    0x000590,    0x0005FF}, 
    {  12,    0x00A500,    0x00A63F}, 
    {  13,    0x000600,    0x0006FF}, 
    {  13,    0x000750,    0x00077F}, 
    {  14,    0x0007C0,    0x0007FF}, 
    {  15,    0x000900,    0x00097F}, 
    {  16,    0x000980,    0x0009FF}, 
    {  17,    0x000A00,    0x000A7F}, 
    {  18,    0x000A80,    0x000AFF}, 
    {  19,    0x000B00,    0x000B7F}, 
    {  20,    0x000B80,    0x000BFF}, 
    {  21,    0x000C00,    0x000C7F}, 
    {  22,    0x000C80,    0x000CFF}, 
    {  23,    0x000D00,    0x000D7F}, 
    {  24,    0x000E00,    0x000E7F}, 
    {  25,    0x000E80,    0x000EFF}, 
    {  26,    0x0010A0,    0x0010FF}, 
    {  26,    0x002D00,    0x002D2F}, 
    {  27,    0x001B00,    0x001B7F}, 
    {  28,    0x001100,    0x0011FF}, 
    {  29,    0x001E00,    0x001EFF}, 
    {  29,    0x002C60,    0x002C7F}, 
    {  29,    0x00A720,    0x00A7FF}, 
    {  30,    0x001F00,    0x001FFF}, 
    {  31,    0x002000,    0x00206F}, 
    {  31,    0x002E00,    0x002E7F}, 
    {  32,    0x002070,    0x00209F}, 
    {  33,    0x0020A0,    0x0020CF}, 
    {  34,    0x0020D0,    0x0020FF}, 
    {  35,    0x002100,    0x00214F}, 
    {  36,    0x002150,    0x00218F}, 
    {  37,    0x002190,    0x0021FF}, 
    {  37,    0x0027F0,    0x0027FF}, 
    {  37,    0x002900,    0x00297F}, 
    {  37,    0x002B00,    0x002BFF}, 
    {  38,    0x002200,    0x0022FF}, 
    {  38,    0x0027C0,    0x0027EF}, 
    {  38,    0x002980,    0x0029FF}, 
    {  38,    0x002A00,    0x002AFF}, 
    {  39,    0x002300,    0x0023FF}, 
    {  40,    0x002400,    0x00243F}, 
    {  41,    0x002440,    0x00245F}, 
    {  42,    0x002460,    0x0024FF}, 
    {  43,    0x002500,    0x00257F}, 
    {  44,    0x002580,    0x00259F}, 
    {  45,    0x0025A0,    0x0025FF}, 
    {  46,    0x002600,    0x0026FF}, 
    {  47,    0x002700,    0x0027BF}, 
    {  48,    0x003000,    0x00303F}, 
    {  49,    0x003040,    0x00309F}, 
    {  50,    0x0030A0,    0x0030FF}, 
    {  50,    0x0031F0,    0x0031FF}, 
    {  51,    0x003100,    0x00312F}, 
    {  51,    0x0031A0,    0x0031BF}, 
    {  52,    0x003130,    0x00318F}, 
    {  53,    0x00A840,    0x00A87F}, 
    {  54,    0x003200,    0x0032FF}, 
    {  55,    0x003300,    0x0033FF}, 
    {  56,    0x00AC00,    0x00D7AF}, 
    {  57,    0x00D800,    0x00DFFF}, 
    {  58,    0x010900,    0x01091F}, 
    {  59,    0x002E80,    0x002EFF}, 
    {  59,    0x002F00,    0x002FDF}, 
    {  59,    0x002FF0,    0x002FFF}, 
    {  59,    0x003190,    0x00319F}, 
    {  59,    0x003400,    0x004DBF}, 
    {  59,    0x004E00,    0x009FFF}, 
    {  59,    0x020000,    0x02A6DF}, 
    {  60,    0x00E000,    0x00F8FF}, 
    {  61,    0x0031C0,    0x0031EF}, 
    {  61,    0x00F900,    0x00FAFF}, 
    {  61,    0x02F800,    0x02FA1F}, 
    {  62,    0x00FB00,    0x00FB4F}, 
    {  63,    0x00FB50,    0x00FDFF}, 
    {  64,    0x00FE20,    0x00FE2F}, 
    {  65,    0x00FE10,    0x00FE1F}, 
    {  65,    0x00FE30,    0x00FE4F}, 
    {  66,    0x00FE50,    0x00FE6F}, 
    {  67,    0x00FE70,    0x00FEFF}, 
    {  68,    0x00FF00,    0x00FFEF}, 
    {  69,    0x00FFF0,    0x00FFFF}, 
    {  70,    0x000F00,    0x000FFF}, 
    {  71,    0x000700,    0x00074F}, 
    {  72,    0x000780,    0x0007BF}, 
    {  73,    0x000D80,    0x000DFF}, 
    {  74,    0x001000,    0x00109F}, 
    {  75,    0x001200,    0x00137F}, 
    {  75,    0x001380,    0x00139F}, 
    {  75,    0x002D80,    0x002DDF}, 
    {  76,    0x0013A0,    0x0013FF}, 
    {  77,    0x001400,    0x00167F}, 
    {  78,    0x001680,    0x00169F}, 
    {  79,    0x0016A0,    0x0016FF}, 
    {  80,    0x001780,    0x0017FF}, 
    {  80,    0x0019E0,    0x0019FF}, 
    {  81,    0x001800,    0x0018AF}, 
    {  82,    0x002800,    0x0028FF}, 
    {  83,    0x00A000,    0x00A48F}, 
    {  83,    0x00A490,    0x00A4CF}, 
    {  84,    0x001700,    0x00171F}, 
    {  84,    0x001720,    0x00173F}, 
    {  84,    0x001740,    0x00175F}, 
    {  84,    0x001760,    0x00177F}, 
    {  85,    0x010300,    0x01032F}, 
    {  86,    0x010330,    0x01034F}, 
    {  87,    0x010400,    0x01044F}, 
    {  88,    0x01D000,    0x01D0FF}, 
    {  88,    0x01D100,    0x01D1FF}, 
    {  88,    0x01D200,    0x01D24F}, 
    {  89,    0x01D400,    0x01D7FF}, 
    {  90,    0x0FF000,    0x0FFFFD}, 
    {  90,    0x100000,    0x10FFFD}, 
    {  91,    0x00FE00,    0x00FE0F}, 
    {  91,    0x0E0100,    0x0E01EF}, 
    {  92,    0x0E0000,    0x0E007F}, 
    {  93,    0x001900,    0x00194F}, 
    {  94,    0x001950,    0x00197F}, 
    {  95,    0x001980,    0x0019DF}, 
    {  96,    0x001A00,    0x001A1F}, 
    {  97,    0x002C00,    0x002C5F}, 
    {  98,    0x002D30,    0x002D7F}, 
    {  99,    0x004DC0,    0x004DFF}, 
    { 100,    0x00A800,    0x00A82F}, 
    { 101,    0x010000,    0x01007F}, 
    { 101,    0x010080,    0x0100FF}, 
    { 101,    0x010100,    0x01013F}, 
    { 102,    0x010140,    0x01018F}, 
    { 103,    0x010380,    0x01039F}, 
    { 104,    0x0103A0,    0x0103DF}, 
    { 105,    0x010450,    0x01047F}, 
    { 106,    0x010480,    0x0104AF}, 
    { 107,    0x010800,    0x01083F}, 
    { 108,    0x010A00,    0x010A5F}, 
    { 109,    0x01D300,    0x01D35F}, 
    { 110,    0x012000,    0x0123FF}, 
    { 110,    0x012400,    0x01247F}, 
    { 111,    0x01D360,    0x01D37F}, 
    { 112,    0x001B80,    0x001BBF}, 
    { 113,    0x001C00,    0x001C4F}, 
    { 114,    0x001C50,    0x001C7F}, 
    { 115,    0x00A880,    0x00A8DF}, 
    { 116,    0x00A900,    0x00A92F}, 
    { 117,    0x00A930,    0x00A95F}, 
    { 118,    0x00AA00,    0x00AA5F}, 
    { 119,    0x010190,    0x0101CF}, 
    { 120,    0x0101D0,    0x0101FF}, 
    { 121,    0x010280,    0x01029F}, 
    { 121,    0x0102A0,    0x0102DF}, 
    { 121,    0x010920,    0x01093F}, 
    { 122,    0x01F000,    0x01F02F}, 
    { 122,    0x01F030,    0x01F09F}, 
};

#endif