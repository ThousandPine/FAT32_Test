#pragma once

#include <string>
#include <stack>

#include "types.h"

/*
 * 目录条目
 */
struct dir_entry
{
    u8 name[11];
    u8 attr;
    u8 NT_res;
    u8 crt_time_tenth;
    u16 crt_time;
    u16 crt_date;
    u16 lst_acc_date;
    u16 fst_clus_hi;
    u16 wrt_time;
    u16 wrt_date;
    u16 fst_clus_lo;
    u32 file_size;
} __attribute__((packed));

/*
 * 长文件名目录条目
 */
struct lfn_entry
{
    u8 ord;
    u8 name1[10];
    u8 attr;
    u8 type;
    u8 chk_sum;
    u8 name2[12];
    u16 fst_clus_lo;
    u8 name3[4];
} __attribute__((packed));

/*
 * 目录信息
 */
class dir
{
public:
    static const u8 ATTR_READ_ONLY = 0x01;
    static const u8 ATTR_HIDDEN = 0x02;
    static const u8 ATTR_SYSTEM = 0x04;
    static const u8 ATTR_VOLUME_ID = 0x08;
    static const u8 ATTR_DIRECTORY = 0x10;
    static const u8 ATTR_ARCHIVE = 0x20;
    static const u8 ATTR_LONG_NAME = (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID);
    static const u32 ENTRY_SZIE = 32;
    static const u8 LAST_LONG_ENTRY = 0x40;

    static bool IS_LFN(u8 attr);
    static bool IS_VALID(dir_entry &entry);
    static bool IS_VALID(lfn_entry &entry);
    static bool IS_LAST_ENTRY(lfn_entry &entry);

    dir();
    dir(std::stack<lfn_entry> &lfn_entries, dir_entry &dir_entry);

    bool valid();
    bool is_dir();
    bool is_arc();
    u32 clus();
    const std::string &name();
    std::string to_string();

private:
    std::string _name;
    std::string _short_name;
    u8 _attr;
    u32 _fst_clus;
    bool _err = false;
    std::string _err_msg = "";

    u8 _read_lfn(std::stack<lfn_entry> &lfn_entries);
    void _read_short_name(dir_entry &dir_entry);
};