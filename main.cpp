#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "fat.h"
#include "io.h"

using namespace std;

// string ascii_to_usc2(const string& s)
// {
//     string res{};
//     for(int i = 0; i < s.size(); i++)
//     {
//         res.push_back(s[i]);
//         res.push_back(0x00);
//     }
//     return res;
// }

int main()
{
    string par = "sda3";
    // cout << "输入FAT32分区名(例如sda1):";
    // cin >> par;

    fat32 fat("/dev/" + par);

    std::cout << fat.to_string() << std::endl;

    while (1)
    {
        string cmd, res{};
        cout << "[" << par << "@FAT32 " << fat.pwd() << "]";
        getline(cin, cmd);

        if (cmd.substr(0, 2) == "ls")
        {
            res = fat.list();
        }
        else if (cmd.substr(0, 2) == "cd")
        {
            // res = to_string(cmd.find_first_of(' '));
            // res = cmd.substr(cmd.find_first_of(' ') + 1);
            string path = cmd.substr(1 + cmd.find_first_of(' '));
            if(false == fat.change_dir(path))
            {
                res = "Faild to open path \"" + path + "\"";
            }
        }
        else if (cmd == "pwd")
        {
            res = fat.pwd();
        }
        else
        {
            res = "Unknow cmd " + cmd;
        }
        if (res.size())
            cout << res << endl;
    }
}