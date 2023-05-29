#include <string.h>

#include "fat.h"
#include "io.h"
#include "debug.h"

/*
 * 根据分区信息进行初始化
 */
fat32::fat32(std::string par)
{
    _fd = fd_open_rw(par.c_str());

    fat_boot_sector bs;
    fd_read(_fd, 0, bs);

    _byte_per_sec = bs.byte_per_sec;
    _sec_per_clus = bs.sec_per_clus;
    _fat_fst_sec = bs.rsvd_sec_cnt;
    _root_fst_clus = bs.root_clus;
    _data_fst_sec = _fat_fst_sec + (bs.num_fats * bs.sec_per_fat32);
    _root_fst_sec = _data_fst_sec + (bs.root_clus - 2) * bs.sec_per_clus;

    _cur_path = "/";
    _switch_to_root();
}

/* ======================================================================= */

/*
 * 返回分区信息
 */
std::string fat32::to_string()
{
    std::string s;
    s += "每个扇区的字节数: " + std::to_string(_byte_per_sec) + "\n";
    s += "每个簇的扇区数: " + std::to_string(_sec_per_clus) + "\n";
    s += "FAT表起始扇区号: " + std::to_string(_fat_fst_sec) + "\n";
    s += "数据区起始扇区号: " + std::to_string(_data_fst_sec) + "\n";
    s += "根目录起始扇区号: " + std::to_string(_root_fst_sec) + "\n";
    s += "根目录起始簇号: " + std::to_string(_root_fst_clus) + "\n";
    return s;
}

/* ======================================================================= */

/*
 * 下一个簇号
 */

/*
 * 簇号对应的字节偏移
 */
off_t fat32::_get_byte_offset(u32 clus)
{
    return _byte_per_sec * (_data_fst_sec + (clus - 2) * _sec_per_clus);
}

/* ======================================================================= */

/*
 * 读取单个目录信息
 * 返回目录信息和下一目录偏移位置
 */
std::pair<dir, off_t> fat32::_read_dir(off_t off_begin, off_t off_end)
{
    u32 entries_cnt = 0;
    std::stack<lfn_entry> lfn_entries{};
    dir_entry dir_entry;
    lfn_entry lfn_entry;

    /*
     * 跳过无效的条目
     */
    fd_read(_fd, off_begin, lfn_entry);
    while (off_begin < off_end && !dir::IS_VALID(lfn_entry) && !dir::IS_LAST_ENTRY(lfn_entry))
    {
        off_begin += dir::ENTRY_SZIE;
        fd_read(_fd, off_begin, lfn_entry);
    }

    if (off_begin >= off_end || dir::IS_LAST_ENTRY(lfn_entry))
        return {{}, off_end};

    /*
     * 读取目录信息
     */
    /* LFN */
    while (off_begin < off_end && dir::IS_LFN(lfn_entry.attr))
    {
        lfn_entries.push(lfn_entry);
        off_begin += dir::ENTRY_SZIE;
        fd_read(_fd, off_begin, lfn_entry);
    }
    /* DIR */
    fd_read(_fd, off_begin, dir_entry);

    return {{lfn_entries, dir_entry}, off_begin + dir::ENTRY_SZIE};
}

/*
 * 读取簇内的所有目录信息
 */
std::vector<dir> fat32::_read_dirs(u32 clus)
{
    std::vector<dir> dirs{};
    off_t off_begin = _get_byte_offset(clus);
    off_t off_end = _get_byte_offset(clus + 1);

    while (off_begin < off_end)
    {
        auto &&res = _read_dir(off_begin, off_end);
        off_begin = res.second;
        if (res.first.valid())
            dirs.emplace_back(res.first);
    }
    return dirs;
}

/*
 * 切换到子目录
 */
void fat32::_switch_to_sub_dir(dir &dir)
{
    _cur_clus = dir.clus();
    if (_cur_path != "/")
        _cur_path.push_back('/');
    _cur_path.append(dir.name());
    return;
}

/*
 * 切换到根目录
 */
void fat32::_switch_to_root()
{
    _cur_clus = _root_fst_clus;
    _cur_path = "/";
    return;
}

/*
 * 获取当前目录信息
 */
std::vector<dir> fat32::_get_cur_dirs()
{
    return _read_dirs(_cur_clus);
}

/*
 * 打开当前目录下的子目录
 */
bool fat32::_open_sub_dir(std::string name)
{
    bool result = false;

    for (auto &dir : _get_cur_dirs())
    {
        if (dir.name() == name)
        {
            if (dir.is_dir())
            {
                _switch_to_sub_dir(dir);
                result = true;
            }
            break;
        }
    }

    return result;
}

/* ======================================================================= */

std::string fat32::pwd() { return _cur_path; }

std::string fat32::list()
{
    std::string s{};
    for (auto &dir : _get_cur_dirs())
    {
        s += dir.to_string() + '\n';
    }
    return s;
}

bool fat32::change_dir(std::string path)
{
    return _open_sub_dir(path);
}
