#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "fat.h"
#include "io.h"

using namespace std;

int main()
{
    int fd;
    fat32 fat("/dev/sda3");
    
    std::cout << fat.to_string() << std::endl;
}