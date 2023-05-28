#pragma once

#include <unistd.h>

/*
 * 以读写方式打开并返回文件描述符
 */
int fd_open_rw(char *dir);

/*
 * 将从起始位置偏移指定n字节后的内容写入buffer
 */
template <typename T>
void fd_read(int fd, __off_t offset, T &buffer)
{
    /* 设定偏移 */
    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        perror("fd_read: lseek");
        exit(1);
    }
    /* 读取内容 */
    if (read(fd, &buffer, sizeof(buffer)) == -1)
    {
        perror("fd_read: read");
        exit(1);
    }
}