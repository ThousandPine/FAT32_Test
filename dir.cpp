
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

/*
 * 根据LFN条目设置长文件名
 * 返回条目中的检验和
 */
u8 dir::_read_lfn(std::stack<lfn_entry> &lfn_entries)
{
    u8 checksum = lfn_entries.top().chk_sum;

    _name.clear();
    while (lfn_entries.size())
    {
        auto &lfn = lfn_entries.top();
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
#define FORCE_ASCII
#ifdef FORCE_ASCII
    /* 
     * 通过去除空白符的方式
     * 强制将LFN的USC2字符变为单字节ASCII码
     */
    for(int i = 0; i < _name.size();)
    {
        if(_name[i] == 0x00)
            _name.erase(i, 1);
        else
            ++i;
    }
#endif
    return checksum;
}

/*
 * 根据条目内容设置8.3文件名
 * 去除多余的空格
 */
void dir::_read_short_name(dir_entry &dir_entry)
{
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
    if (_short_name.back() == '.')
        _short_name.pop_back();
    return;
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
    /* 8.3文件名 */
    _read_short_name(dir_entry);
    /* LFN */
    if (lfn_entries.size())
    {
        checksum = _read_lfn(lfn_entries);
        /* 文件名检验和 */
        if (checksum != lfn_checksum(dir_entry.name))
        {
            _err = true;
            _err_msg.append("WARNING::LFN校验和出错");
        }
    }
    else
        _name = _short_name;

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
const std::string &dir::name() { return _name; }

std::string dir::to_string()
{
    std::string s = _name + "(" + _short_name + ")" + std::to_string(_fst_clus);
    if (_err)
        s.append(_err_msg);
    return s;
}
