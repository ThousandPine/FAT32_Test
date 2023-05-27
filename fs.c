#include "fs.h"
#include "io.h"

/* 
 * 根据引导扇区的BPB，构造FAT32分区基本信息
 */
void fat_init_fat32_info(int fd, struct fat32_info *fat)
{
    struct fat_boot_sector bs;

    FD_READ(fd, 0, bs);

    /* 计算参数 */
    fat->sector_size = bs.byte_per_sec;
    fat->sec_per_clus = bs.sec_per_clus;
    fat->fat_sec = bs.rsvd_sec_cnt;
    fat->data_sec = fat->fat_sec + (bs.num_fats * bs.sec_per_fat32);
    fat->root_sec = fat->data_sec + (bs.root_clus - 2) * bs.sec_per_clus;
}