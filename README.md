# FAT32 TEST

## 简介

Linux下实现FAT32文件系统管理的简单程序。

功能还在陆续开发中，当前进度：

[x] 读取引导扇区BPB信息

[x] 访问根目录

[x] 输出8.3文件名

[x] 输出VFAT长文件名(LFN)

## 使用方式

输入`make run`即可运行。

## 参考资料

[How FAT Works: Local File Systems | Microsoft Learn](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2003/cc776720(v=ws.10))

[文件分配表 - 维基百科，自由的百科全书](https://zh.wikipedia.org/wiki/%E6%AA%94%E6%A1%88%E9%85%8D%E7%BD%AE%E8%A1%A8)

[8.3文件名 - 维基百科，自由的百科全书](https://zh.wikipedia.org/wiki/8.3%E6%96%87%E4%BB%B6%E5%90%8D)

[Design of the FAT file system - Wikipedia](https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#Directory_entry)

[FAT32 File System Specification](https://web.archive.org/web/20071013033802/http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx)

[一步一步了解FAT32文件系统 · Mingrui (Ray) Zhang](https://drustz.com/posts/2015/11/17/filesystem/)