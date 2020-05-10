/*******************************************************************************************
 *Justin Pulido
 *Lab2
 *Sdisk.h
 *Sdisk.h will be holding responsible for declaring the correct information for the 
 *.cpp file to be used and accessing the private member functions as well. 
*******************************************************************************************/

#ifndef SDISK_H
#define SDISK_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Sdisk
{
	public:
		Sdisk(string diskname, int numbers_of_blocks, int blocks_size);
		int get_block(int block_number, string& buffer);
		int put_block(int block_number, const string& buffer);
		int get_number_of_blocks(); // accessor function
		int get_block_size(); // accessor function
	private:
		string diskname;		//file name of the softare disk
		int number_of_blocks;	//number of blocks on disk
		int block_size; 		//block size in bytes
};

#endif
