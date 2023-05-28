#pragma once

#include <string>
#include <vector>

#include "types.h"
#include "dir.h"

/*
 * FAT32引导扇区信息
 * 关闭内存对齐优化，方便一次性读取赋值
 */
struct fat_boot_sector
{
    u8 jump_ins[3];
    u8 OEM[8];
    /* BPB */
    u16 byte_per_sec;
    u8 sec_per_clus;
    u16 rsvd_sec_cnt;
    u8 num_fats;
    u16 root_entries;
    u16 small_sec;
    u8 media;
    u16 sec_per_fat16;
    u16 sec_per_track;
    u16 num_heads;
    u32 hidd_sec;
    u32 tot_sec;
    /* FAT32 EBPB */
    u32 sec_per_fat32;
    u16 extflags;
    u16 fs_version;
    u32 root_clus;
    u16 fs_info;
    u16 back_boot_sec;
    u8 fat32_rsvd_0[12];
    u8 drive_num;
    u8 fat32_rsvd_1;
    u8 boot_sig;
    u32 fat32_vol_id;
    u8 fat32_vol_lab[11];
    u8 fs_type[8];
} __attribute__((packed));

/*
 * FAT32分区
 */
class fat32
{
public:
    fat32(std::string par);

    /*
     * 返回分区信息
     */
    std::string to_string();

    /*
     * 打开文件夹并返回信息
     */
    std::vector<dir> open_dir(std::string dir);

    std::vector<dir> open_root();

private:
    int _fd; /* 分区文件描述符 */

    u32 _byte_per_sec;  /* 每个扇区的字节数 */
    u32 _sec_per_clus;  /* 每个簇的扇区数 */
    u32 _fat_fst_sec;   /* FAT表起始扇区号 */
    u32 _data_fst_sec;  /* 数据区起始扇区号 */
    u32 _root_fst_sec;  /* 根目录起始扇区号 */
    u32 _root_fst_clus; /* 根目录起始簇号 */

    off_t _get_byte_offset(u32 clus);
    std::pair<dir, off_t> _read_dir(off_t begin, off_t end);
    std::vector<dir> _read_all_dir(u32 clus);
};
