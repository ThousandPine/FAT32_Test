#include "bpb.h"
#include "io.h"

/*
 * 从磁盘中读取BPB
 */
static void fat_read_bpb(int fd, struct bios_param_block *bpb)
{
    FD_READ(fd, 0xb, bpb->sector_size);
    FD_READ(fd, 0xd, bpb->sec_per_clus);
    FD_READ(fd, 0xe, bpb->reserved);
    FD_READ(fd, 0x10, bpb->fats);
    FD_READ(fd, 0x24, bpb->fat32_fat_size);
    FD_READ(fd, 0x2c, bpb->fat32_root_cluster);
}

/* 
 * 根据BPB内容，构造FAT32分区基本信息
 */
void fat_init_fat32_info(int fd, struct fat32_info *fat)
{
    struct bios_param_block bpb;

    fat_read_bpb(fd, &bpb);

    /*
     * 计算参数
     */
    fat->sector_size = bpb.sector_size;
    fat->sec_per_clus = bpb.sec_per_clus;
    fat->fat_sec = bpb.reserved;
    fat->data_sec = fat->fat_sec + (bpb.fats * bpb.fat32_fat_size);
    fat->root_sec = fat->data_sec + (bpb.fat32_root_cluster - 2) * bpb.sec_per_clus;
}