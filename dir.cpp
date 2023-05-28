
#include "types.h"

/*
 * LFN检验和
 */
static u8 lfn_checksum(const u8 *name)
{
    u8 sum = 0;

    for (int i = 11; i; i--)
        sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *name++;
    return sum;
}