#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#include "bpb.h"

#define debug(x) printf("debug::%s: 0x%x\n", #x,(x));

int main()
{
    int fd;
    struct fat32_info fat;

    fd = open("/dev/sda3", O_RDONLY); //以只读方式打开设备文件
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    fat_init_fat32_info(fd, &fat);

    debug(fat.sector_size);
    debug(fat.sec_per_clus);
    debug(fat.fat_sec);
    debug(fat.data_sec);
    debug(fat.root_sec);
}