#include <string.h>

#include "fat.h"
#include "io.h"
#include "debug.h"

/*
 * 根据分区信息进行初始化
 */
fat32::fat32(std::string par)
{
    this->_fd = fd_open_rw(par.c_str());

    fat_boot_sector bs;
    fd_read(this->_fd, 0, bs);

    this->_byte_per_sec = bs.byte_per_sec;
    this->_sec_per_clus = bs.sec_per_clus;
    this->_fat_fst_sec = bs.rsvd_sec_cnt;
    this->_root_fst_clus = bs.root_clus;
    this->_data_fst_sec = this->_fat_fst_sec + (bs.num_fats * bs.sec_per_fat32);
    this->_root_fst_sec = this->_data_fst_sec + (bs.root_clus - 2) * bs.sec_per_clus;
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
    while (off_begin < off_end && !dir::is_valid(lfn_entry))
    {
        off_begin += dir::ENTRY_SZIE;
        fd_read(_fd, off_begin, lfn_entry);
    }

    if (off_begin >= off_end)
        return {{}, off_begin};

    /*
     * 读取目录信息
     */
    /* LFN */
    while (off_begin < off_end && dir::is_lfn(lfn_entry.attr))
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
 * 读取簇号内的所有目录信息
 */
std::vector<dir> fat32::_read_all_dir(u32 clus)
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
 * 打开文件夹并返回信息
 */
std::vector<dir> fat32::open_dir(std::string dir)
{
    return {};
}

std::vector<dir> fat32::open_root()
{
    return _read_all_dir(this->_root_fst_clus);
}
