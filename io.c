#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 读取从起始位置偏移指定n字节后的内容
 */
void fd_read(int fd, __off_t offset, void *buffer, size_t bytes)
{
    /* 设定偏移 */
    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        perror("fd_read: lseek");
        exit(1);
    }
    /* 读取内容 */
    if (read(fd, buffer, bytes) == -1) {
        perror("fd_read: read");
        exit(1);
    }
}