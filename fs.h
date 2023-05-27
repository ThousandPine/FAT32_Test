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
struct fat_info {
    u32	byte_per_sec;    	/* 每个扇区的字节数 */
	u32	sec_per_clus;   	/* 每个簇的扇区数 */
    u32 fat_fst_sec;        /* FAT表起始扇区号 */
    u32 data_fst_sec;       /* 数据区起始扇区号 */
    u32 root_fst_sec;       /* 根目录起始扇区号 */
};

/* 
 * 根据BPB内容，构造FAT32分区基本信息
 */
void fat_init_fat_info(int fd, struct fat_info *fat);

/* ========================================================== */

/* 
 * 目录条目
 */
struct dir_entry {
	u8	name[11];
	u8	attr;
	u8 	NT_res;
	u8	crt_time_tenth;
	u16	crt_time;
	u16	crt_date;
	u16 lst_acc_date;
	u16	fst_clus_hi;
	u16	wrt_time;
	u16	wrt_date;
	u16 fst_clus_lo;
	u32	file_size;
}__attribute__((packed));

/* 
 * 长文件名目录条目
 */
struct LFN_entry {
	u8	ord;
	u8	name1[10];
	u8	attr;
	u8	type;
	u8	chk_sum;
	u8	name2[12];
	u16	fst_clus_lo;
	u8	name3[4];
}__attribute__((packed));

/*
 * 目录主要信息
 * 由目录条目信息构造得到
 */
struct dir_info {

};