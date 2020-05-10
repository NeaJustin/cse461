/*******************************************************************************************
 *Justin Pulido
 *Sdisk.cpp
 *Sdisk.cpp will be used for creating the constructor, being able to get the block number, 
 *and getting the block size as well. It will then write to the file that is open and 
 *read from the file as well. This is so that it can 
*******************************************************************************************/

#include "Sdisk.h"
#include "Filesys.h"

/********************************************************************
 *will be responsible for getting the number of blocks, block size,
 *and will be getting the diskname as well so that it can write
 *into the file and read from it. It then also opens and closes the 
  file once it is done with it. 
*********************************************************************/
Sdisk::Sdisk(string diskname, int numbers_of_blocks, int blocks_size)
{
    //setting diskname, block size, and number of blocks. 
	this->diskname = diskname;
    this->block_size = blocks_size;
    this->number_of_blocks = numbers_of_blocks;

    //getting total number of blocks.     
    int total_blocks = number_of_blocks * block_size;

    //creating file stream
    //opening the file and converting it to a c string and 
    //having both reading in and writing out. 
	fstream infile;
	infile.open(diskname.c_str(), std::fstream::in | std::fstream::out);	

    //checking if the file is good and if it isnt close the file. 
	if(!infile.good()) {
		std::cout << "Could not find diskname: " << diskname << endl;
        infile.close();
		//creating file here
		infile.open(diskname.c_str(), std::ofstream::out | std::ofstream::app);
		std::cout << "created disk: " << diskname << endl;
		return;
	}	

    //if the file is good, write to the file with #  
    if(infile.good()) {
        cout << "diskname: " << diskname << " found." << endl;
        for(int i = 0; i < total_blocks; i++) {
            infile << "#";
        }
        infile.close();
    }
}

/*************************************************************
 *get_block will be getting the returning the size of the 
 *buffer from sdisk 
*************************************************************/
int Sdisk::get_block(int block_number, string& buffer)
{
    ifstream infile;
    infile.open(diskname.c_str(), std::ifstream::in);  
	
	//putting to the correct block number that we need
	infile.seekg(block_number*block_size);
	
	//starting from the block number and getting the size which of it
	//and adding to the buffer by using .push_back
    for(int i = 0; i < block_size; i++) {
        //reading the buffer and the block size of it one character 
		buffer.push_back(infile.get());
    }

	//closing the file. 
    infile.close();

	return 1;
}

/*************************************************************
 *responsible for putting the block in the correct spot in 
 *the txt file that is created. using output stream to 
 *write the correct values in the correct spots. 
 *doing this on little sleep please dont judge wording. 
*************************************************************/
int Sdisk::put_block(int block_number, const string& buffer)
{
    ofstream outstream;
    outstream.open(diskname.c_str(), std::ofstream::in | std::ofstream::out);
    int temp = (block_size*block_number);
    outstream.seekp(temp);
    outstream.write(buffer.c_str(), buffer.size());  
   
    outstream.close(); 
    return 1;
}

int Sdisk::get_number_of_blocks()
{
	return number_of_blocks;
}

int Sdisk::get_block_size()
{
	return block_size;
}
