#pragma once

#include "types.h"

/*
 * FAT32引导扇区信息
 * 关闭内存对齐优化，方便一次性读取赋值
 */
struct fat_boot_sector {
	u8	jump_ins[3];
	u8	OEM[8];
	/* BPB */
	u16 byte_per_sec;
	u8	sec_per_clus;
	u16	rsvd_sec_cnt;
	u8 	num_fats;
	u16 root_entries;
	u16 small_sec;
	u8 	media;
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
	u8 	fat32_rsvd_0[12];
	u8 	drive_num;
	u8	fat32_rsvd_1;
	u8 	boot_sig;
	u32 fat32_vol_id;
	u8 	fat32_vol_lab[11];
	u8	fs_type[8];
}__attribute__((packed));

/* 
 * FAT32分区主要信息
 */
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