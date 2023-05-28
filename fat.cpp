
#include "fat.h"
#include "io.h"

/*
 * 根据分区信息进行初始化
 */
fat32::fat32(char *par)
{
    this->_fd = fd_open_rw(par);

    fat_boot_sector bs;
    fd_read(this->_fd, 0, bs);

    this->_byte_per_sec = bs.byte_per_sec;
    this->_sec_per_clus = bs.sec_per_clus;
    this->_fat_fst_sec = bs.rsvd_sec_cnt;
    this->_data_fst_sec = this->_fat_fst_sec + (bs.num_fats * bs.sec_per_fat32);
    this->_root_fst_sec = this->_data_fst_sec + (bs.root_clus - 2) * bs.sec_per_clus;
}

std::string fat32::to_string()
{
  std::string s;
  s += "每个扇区的字节数: " + std::to_string(_byte_per_sec) + "\n";
  s += "每个簇的扇区数: " + std::to_string(_sec_per_clus) + "\n";
  s += "FAT表起始扇区号: " + std::to_string(_fat_fst_sec) + "\n";
  s += "数据区起始扇区号: " + std::to_string(_data_fst_sec) + "\n";
  s += "根目录起始扇区号: " + std::to_string(_root_fst_sec) + "\n";
  return s;
}