#pragma once

#include <unistd.h>

#define FD_READ(fd, offset, buff) fd_read(fd, offset, &(buff), sizeof(buff))

/*
 * 读取从起始位置偏移指定n字节后的内容
 */
void fd_read(int fd, __off_t offset, void *buffer, size_t bytes);