
/***************************************************************************
 * Justin Pulido
 * Lab 5 & 6
 * Shell.cpp
 * will be responsible for creating the constructor, adding a new file,
 * listing the contents, deleting files, listing types and copying a 
 * file from one to another. 
***************************************************************************/

#include "Shell.h"


Shell::Shell(std::string filename, int block_size, int number_of_blocks):Filesys(filename, number_of_blocks, block_size){}

//dir will be listing all the files in the directory
int Shell::dir()
{
	std::vector<std::string> flist = ls();
	for (int i = 0; i < flist.size(); ++i) {
		std::cout << flist[i] << std::endl;
	}
	return 1;
}

//add will add a new file using input from the keyboard
int Shell::add(std::string file)
{
	//8 is the size of a block
	int x = 8;
	if (file.size() > x) {
		file = file.substr(0, x);
	}
	return new_file(file);
}

//deletes the file it is currently at. 
int Shell::del(std::string file)
{
	int block = get_first_block(file);
	while (block > 0) {
		del_block(file, block);
		block = get_first_block(file);
	}
	return rm_file(file);
}

//lists the content of the file. 
int Shell::type(std::string file)
{
	int block = get_first_block(file);
	string buffer;
	while (block > 0) {
		string t;
		int error = read_block(file, block, t);
		buffer += t;
		block = next_block(file, block);
	}
	std::cout << buffer << std::endl;
	return 1;
}

//copies file1 to file2 and thats it. 
int Shell::copy(std::string file1, std::string file2)
{
	int code = get_first_block(file1);
	if (code == -1) {
		return 0;
	}

	int code2 = get_first_block(file2);
	if (code2 == -1) {
		std::cout << "file 2 exists" << std::endl;
		return 0;
	}

	int code3 = new_file(file2);
	if (code3 == 0) {
		std::cout << "no space in root directory" << std::endl;
		return 0;
	}

	int iblock = code; // first block
	while (iblock != 0) {
		std::string b;
		get_block(0, b);
		int code4 = add_block(file2, b);
		if (code4 == 1) {
			std::cout << "no space left" << std::endl;
			del(file2);
			return 0;
		}
		iblock = next_block(file1, iblock);
	}

	return 1;
}

