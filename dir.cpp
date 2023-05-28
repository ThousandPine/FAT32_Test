
#include "types.h"
#include "dir.h"
#include "debug.h"

bool dir::is_lfn(u8 attr)
{
    return (attr == ATTR_LONG_NAME);
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
        int i = _short_name.find_first_of('.') - 1;
        if (_short_name[i] == ' ')
            _short_name.erase(i, 1);
        else
            break;
    }
    while (_short_name.back() == ' ' || _short_name.back() == '.')
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
}

/* ============================================ */

bool dir::valid() { return !_err; }

std::string dir::to_string()
{
    std::string s = _name + "(" + _short_name + ")";
    if(_err)
        s.append(_err_msg);
    return s;
    // return _name;
}
