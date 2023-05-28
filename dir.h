
#include "types.h"

/*
 * 目录属性
 */
#define ATTR_READ_ONLY	0x01
#define ATTR_HIDDEN		0x02
#define ATTR_SYSTEM		0x04
#define ATTR_VOLUME_ID	0x08
#define ATTR_DIRECTORY 	0x10
#define ATTR_ARCHIVE 	0x20
#define ATTR_LONG_NAME 	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

#define DIR_ENTRY_SIZE	32

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

#define LAST_LONG_ENTRY	0x40

/* 
 * 长文件名目录条目
 */
struct lfn_entry {
	u8	ord;
	u8	name1[10];
	u8	attr;
	u8	type;
	u8	chk_sum;
	u8	name2[12];
	u16	fst_clus_lo;
	u8	name3[4];
}__attribute__((packed));