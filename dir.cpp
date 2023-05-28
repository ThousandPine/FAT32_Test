
#include "types.h"
#include "dir.h"

bool dir::is_lfn(u8 attr)
{
    return (attr & ATTR_LONG_NAME) != 0;
}
bool dir::is_valid(dir_entry &entry)
{
    return entry.name[0] != 0xe5 && entry.name[0] != 0x00;
}
bool dir::is_valid(lfn_entry &entry)
{
    return entry.ord != 0xe5 && entry.ord != 0x00;
}

/* ============================================ */

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

/* ============================================ */

dir::dir(std::vector<lfn_entry> &lfn_entries, dir_entry &dir_entry)
{
    _name = "";
    for(int i = 0; i < 11; i++)
    {
        _name.push_back(dir_entry.name[i]);
    }
}

/* ============================================ */

std::string dir::to_string()
{
    return _name;
}
