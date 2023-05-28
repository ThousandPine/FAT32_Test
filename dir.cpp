
#include "types.h"
#include "dir.h"
#include "debug.h"

bool dir::IS_LFN(u8 attr)
{
    return (attr == ATTR_LONG_NAME);
}
bool dir::IS_LAST_ENTRY(lfn_entry &entry)
{
    return entry.ord == 0x00;
}
bool dir::IS_VALID(dir_entry &entry)
{
    return entry.name[0] != 0xe5 && entry.name[0] != 0x00;
}
bool dir::IS_VALID(lfn_entry &entry)
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

dir::dir()
{
    _err = true;
    _err_msg.append("WARNING::数据为空");
}

dir::dir(std::stack<lfn_entry> &lfn_entries, dir_entry &dir_entry)
{
    /*
     * 设置文件名
     */
    u8 checksum;
    bool has_lfn = !lfn_entries.empty();
    /* LFN */
    _name.clear();
    while (lfn_entries.size())
    {
        auto &lfn = lfn_entries.top();
        checksum = lfn.chk_sum;
        _name.append((const char *)lfn.name1, 10);
        _name.append((const char *)lfn.name2, 12);
        _name.append((const char *)lfn.name3, 4);
        lfn_entries.pop();
    }
    for (int i = 0; i < _name.size(); i += 2)
    {
        if (_name[i] == 0x00)
        {
            _name = _name.substr(0, i);
            break;
        }
    }
    /* 8.3文件名 */
    _short_name.clear();
    _short_name.append((const char *)dir_entry.name, 11);
    _short_name.insert(8, ".");
    while (1)
    {
        int i = _short_name.find_last_of('.') - 1;
        if (_short_name[i] == ' ')
            _short_name.erase(i, 1);
        else
            break;
    }
    while (_short_name.back() == ' ')
        _short_name.pop_back();
    if(_short_name.back() == '.')
        _short_name.pop_back();

    if (!has_lfn)
        _name = _short_name;

    /* 文件名检验和 */
    if (has_lfn && checksum != lfn_checksum(dir_entry.name))
    {
        _err = true;
        _err_msg.append("WARNING::LFN校验和检测出错");
    }

    /*
     * 其他参数
     */
    this->_attr = dir_entry.attr;
    this->_fst_clus = dir_entry.fst_clus_lo | (((u32)dir_entry.fst_clus_hi) << 16);
}

/* ============================================ */

bool dir::valid() { return !_err; }
bool dir::is_dir() { return 0 != (_attr & ATTR_DIRECTORY); }
bool dir::is_arc() { return 0 != (_attr & ATTR_ARCHIVE); }

u32 dir::clus() { return _fst_clus; }
std::string dir::name() { return _name; }

std::string dir::to_string()
{
    std::string s = _name + "(" + _short_name + ")" + std::to_string(_fst_clus);
    if (_err)
        s.append(_err_msg);
    return s;
}
