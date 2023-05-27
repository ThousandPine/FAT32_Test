#pragma once

#include "types.h"

/* 
 * FAT分区引导扇区中的BIOS参数块
 * 暂时只记录FAT32需要的部分重要信息
 */
struct bios_param_block {
	u16	sector_size;    /* 每个扇区的字节数 */
	u8	sec_per_clus;   /* 每个簇的扇区数 */
	u16	reserved;       /* 保留扇区数（包括引导扇区） */
	u16	fats;           /* FAT表数量 */

    /* 仅FAT32 */
	u32	fat32_fat_size;     /* 每个FAT表所占扇区 */
	u32	fat32_root_cluster; /* 根目录所在簇号 */
};

struct fat32_info {
    u32	sector_size;    /* 每个扇区的字节数 */
	u32	sec_per_clus;   /* 每个簇的扇区数 */
    u32 fat_sec;        /* FAT表起始扇区号 */
    u32 data_sec;       /* 数据区起始扇区号 */
    u32 root_sec;       /* 根目录起始扇区号 */
};

/* 
 * 根据BPB内容，构造FAT32分区基本信息
 */
void fat_init_fat32_info(int fd, struct fat32_info *fat);