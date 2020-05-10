/*****************************************************************************
 * Justin Pulido
 * Lab 5 & 6
 * 11/18/19
 * Shell.h
 * Shell.h will hold the member functions and inherit from the Filesys
 * that was created. 
*****************************************************************************/

#ifndef SHELL_H
#define SHELL_H
#include "Filesys.h"

class Shell : public Filesys
{
	public:
		Shell(std::string filename, int block_size, int number_of_blocks);
		int dir(); //list all files
		int add(std::string file);
		int del(std::string file);
		int type(std::string file);
		int copy(std::string file1, std::string file2); 
};

#endif
