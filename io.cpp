#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * 以读写方式打开并返回文件描述符
 */
int fd_open_rw(const char *dir)
{
    int fd = open(dir, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    return fd;
}