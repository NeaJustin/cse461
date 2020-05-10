#ifndef FILESYS_H
#define FILESYS_H
#include "Sdisk.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

class Filesys: public Sdisk
{
    public:
        Filesys(string diskname, int number_of_blocks, int block_size);
        int fclose();
        int fssynch();
        int new_file(string file);
        int rm_file(string file);
        int get_first_block(string file);
        int add_block(string file, string block);
		int del_block(string file, int block_number);
        int read_block(string file, int block_number, string& buffer);
        int write_block(string file, int block_number, string buffer);
        int next_block(string file, int block_number);
        int get_root_size();
        int get_fat_size();
        bool check_block(string block, int block_number);
		vector<string> ls();
    private:
        int root_size;
        int fat_size;
        vector<string> filename;
        vector<int> first_block;
        vector<int> fat;
};

vector<string>block(string buffer, int b);

#endif
