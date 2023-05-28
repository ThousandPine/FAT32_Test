#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "fat.h"
#include "io.h"

using namespace std;

int main()
{
    string par;
    cout << "输入FAT32分区名(例如sda1):";
    cin >> par;

    fat32 fat("/dev/" + par);
    
    std::cout << fat.to_string() << std::endl;

    auto &&dirs = fat.open_root();
    cout << "root_dir::" << endl;
    for(auto &dir : dirs)
    {
        cout << dir.to_string() << endl;
    }
}