// NOTE(furkan): This implementation does not belong to 
//               me, yet I could not find where I got it.

const unsigned int CRC32_REVERSE = 0xEDB88320;
      unsigned int CRC32_TABLE[256];

void crc32b_init() {
    for( int byte = 0; byte < 256; byte++ ){
        unsigned int crc = (unsigned int) byte; // reflected form

        for( char bit = 0; bit < 8; bit++ ){
            if(crc & 1){
                crc = (crc >> 1) ^ CRC32_REVERSE; // reflected Form
            } else {
                crc = (crc >> 1);
            }
        }

        CRC32_TABLE[ byte ] = crc;
    }

    if( CRC32_TABLE[128] != CRC32_REVERSE ){
        printf("ERROR: CRC32 Table not initialized properly!\n");
    }
}

unsigned int crc32b_table_reflect( int nLength, const unsigned char *pData ) {
    unsigned int crc = -1 ; // Optimization: crc = CRC32_INIT;
    
    while( nLength-- > 0 ){
        crc = CRC32_TABLE[ (crc ^ *pData++) & 0xFF ] ^ (crc >> 8); // reflect: >> 8
    }
    
    return ~crc; // Optimization: crc ^= CRC32_DONE
} 