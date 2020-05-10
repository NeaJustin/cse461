#ifndef TABLE_H
#define TABLE_H

#include "Filesys.h"

class Table : Public Filesys
{
	public:
		Table(std::string diskname, int block_size, int number_of_blocks, std::string flat_file, std::string index_file);
		int build_table(std::string input_file);
		int search(std::string value);
		std::string get_file() { return flat_file; }
		std::string get_index_file() { return index_file; }
		int get_number_of_blocks() { return number_of_blocks; }
	Private:
		std::string flat_file;
		std::string index_file;
		Filesys filesystem;
		int number_of_blocks;
		int index_search(std::string value);
};

#endif
