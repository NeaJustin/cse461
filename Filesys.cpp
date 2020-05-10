#include "Filesys.h"
#include "Sdisk.h"
#include <string>

vector<string> block(string buffer, int b) 
{
	vector<string> blocks;
	int number_of_blocks = 0;
	
	if (buffer.length() % b == 0) {
		number_of_blocks = buffer.length() / b;
	} else {
		number_of_blocks = buffer.length() / b + 1;
	}

	string temp_block;
	for (int i = 0; i < number_of_blocks; i++) {
		temp_block = buffer.substr(b*i, b);
		blocks.push_back(temp_block);
	}

	int last_block = blocks.size() - 1;
	for (int i = blocks[last_block].length(); i < b; i++) {
blocks[last_block] += "#";
	}

	return blocks;
}

/*********************************************************************
 * the constructor will read the incoming file and will check to
 * see if the file exists or not. It will then create the root size
 * and the fat size and from there it will create a file system if
 * there is no current file with that specific name.
 * if the file system is there, it will read it and make the
 * changes that are needed for it and to return the correct output
 * that is needed in writing to the files in the filesystem from the
 * SDisk.
**********************************************************************/
Filesys::Filesys(string diskname, int number_of_blocks, int block_size) :Sdisk(diskname, number_of_blocks, block_size)
{
	string buffer;
	string next_thing;

	root_size = get_block_size() / 13;
	fat_size = (4 * get_number_of_blocks()) / get_block_size() + 1;
	get_block(1, buffer);

	if (buffer[0] == '#') {

		for (int i = 0; i < root_size; i++) {
			filename.push_back("xxxxxxxx");
			first_block.push_back(0);
		}

		fat.push_back(fat_size);
		fat.push_back(0);

		for (int i = 2; i < fat_size; ++i) {
			fat.push_back(0);
		}

		for (int i = fat_size + 1; i < number_of_blocks; i++) {
			fat.push_back(i + 1);
		}
		fat[fat.size() - 1] = 0;

		fssynch();
		cout << "file is now created" << endl;
	}
	else {
		istringstream fstream1;
		istringstream fstream2;

		ostringstream string_output;
		string buffer1;
		get_block(0, buffer1);
		fstream1.str(buffer1);

		for (int i = 0; i < root_size; ++i) {
			string m;
			int x;
			fstream1 >> m >> x;
			filename.push_back(m);
			first_block.push_back(x);
		}

		string new_buffer;

		for (int i = 0; i <= fat_size; i++) {
			string temp;
			get_block(i, temp);
			new_buffer += temp;
		}

		fstream2.str(new_buffer);
		for (int i = 0; i < number_of_blocks; ++i) {
			int z = 0;
			fstream2 >> z;
			fat.push_back(z);
		}
	}
	fssynch();
}

/************************************************
 * synchs the file
************************************************/
int Filesys::fclose()
{
	fssynch();
	return 1;
}

/*************************************************
 * will write to the file and it will then make
 * make the proper changes that are required
 * of it so that the file is then up to date.
*************************************************/
int Filesys::fssynch()
{
	string buffer, fat_buffer;
	ostringstream output; //fat stream
	ostringstream root_stream;
	
	for (int i = 0; i < root_size; ++i) {
		root_stream << filename[i] << " " << first_block[i] << " ";
	}

	/*getting fat*/
	for (int i = 0; i < fat.size(); i++) {
		output << fat[i] << " ";
	}

	buffer = root_stream.str();
	fat_buffer = output.str();
	vector<string> root_creation;
	vector<string> fat_creation;

	root_creation = block(buffer, get_block_size());
	fat_creation = block(fat_buffer, get_block_size());

	put_block(0, root_creation[0]);


	for (int i = 1; i <= fat_creation.size(); ++i) {
		put_block(i, fat_creation[i - 1]);
	}

    return 1;
}

/**********************************************************
 * New file will check to see if the file is already 
 * created, if it is not then go ahead and create the
 * file. if it is there then say the file already exists. 
**********************************************************/
int Filesys::new_file(string file)
{
	//cout << root_size << endl;
	for (int i = 0; i < filename.size(); i++) {
		//cout << "entering shit 1 now." << endl;
		if (filename[i] == file) {
			cout << "file already exists" << endl;
			return 0;
		}
	}

	for (int i = 0; i < filename.size(); i++) {
		//cout << "entering shit 2 now." << endl;
		if (filename[i] == "xxxxxxxx") {
			filename[i] = file;
			fssynch();
			return 1;
		}
	}
    return 0;
}

/********************************************************
 * Remove file will be responsible for removing the file
 * if needed. if the file is not there, then do not do 
 * anything since the file does not exist. 
********************************************************/
int Filesys::rm_file(string file)
{
	for (int i = 0; i < root_size; i++) {
		if (filename[i] == file) {
			if (first_block[i] != 0) {
				cout << "file is not empty" << endl;
				return 0;
			} else {
				filename[i] == "xxxxxxxx";
				fssynch();
				return 1;
			}
		}
	}
    return 0;
}

/*******************************************************
 * returns the first block of the file
*******************************************************/
int Filesys::get_first_block(string file)
{
	for (int i = 1; i < filename.size(); i++) {
		if (filename[i] == file) {
			return first_block[i];
		}
	}
    return 1;
}

/*********************************************************
 * adds a block of data stored in the buffer string 
 * to the end of the file and returns the block number. 
*********************************************************/
int Filesys::add_block(string file, string block)
{
	string buffer;
	int first = get_first_block(file);
	if (first == -1) {
		return 0;
	}

	int allocate = fat[0];
	//checking to see if there are blocks available. 
	if (allocate == 0) {
		//no free blocks
		cout << "no free blocks available" << endl;
		return 0;
	}

	fat[0] = fat[fat[0]];
	fat[allocate] = 0;

	//if first is free
	if (first == 0) {
		for (int i = 0; i < root_size; i++) {
			if (filename[i] == file) {
				cout << "entering loop" << endl;
				first_block[i] = allocate;
				put_block(allocate, buffer);
				fssynch();
				return allocate;
			}
		}
		
	} else {
		int block = first;
		while (block != 0) {
			if(fat[block] != 0) { //fat[1] == -1?
				block = fat[block];
			}
			else {
				break;
			}
			fat[block] = allocate;
			put_block(allocate, buffer);
			fssynch();
			return allocate;
		}
	}

	return allocate;
}

/*********************************************************
 * will go to the block number specifically and then 
 * it will remove that block from the file and 
 * synch after. 
*********************************************************/
int Filesys::del_block(string file, int block_number)
{
	if (block_number == get_first_block(file)) {
		for (int i = 0; i < filename.size(); ++i) {
			if (filename[i] == file) {
				first_block[i] = fat[block_number];
			}
		}
	}

    int remov = block_number;
    if(block_number == get_first_block(file)) {
        for(int i = 0; i < filename.size(); ++i) {
            if(file == filename[i]) {
                if(next_block(file, block_number) != -1) {
                    first_block[i] = next_block(file, block_number);
                } else {
                    first_block[i] = fat[remov];
                    break;
                }
            }
        }
    }
    else {
        int iblock = get_first_block(file);
        while(fat[iblock] != remov) {
            iblock = fat[iblock];
        }
        fat[iblock] = fat[remov];
    }
    fat[remov] = fat[0];
    fat[0] = remov;
	fssynch();
    return 1;
}

/*********************************************************
 * gets the block number and stores it in the buffer and 
 * returns correct or not depending if it is their 
*********************************************************/
int Filesys::read_block(string file, int block_number, string& buffer)
{
    if(check_block(file, block_number)) {
    	get_block(block_number, buffer);
        return 1;
    }
    return 0;
}

/*********************************************************
 * writes buffer to block number in the correct location. 
*********************************************************/
int Filesys::write_block(string file, int block_number, string buffer)
{
    if(check_block(file, block_number)) {
        put_block(block_number, buffer);
        return 1;
    }
    return 0;
}

/*********************************************************
 * returns the number of blocks that follows the 
 * block_number. 
*********************************************************/
int Filesys::next_block(string file, int block_number)
{
	int block = get_first_block(file);

	bool correct = true;
	while (correct) {
		if (block == block_number) {
			if (fat[block] == 0) {
				return 0;
			} else {
				return fat[block];
			}
		}
		if (fat[block] == 0) {
			correct = false;
		}

		block = fat[block];
	}

    return -1;
}

/*********************************************************
 * returning root_size
*********************************************************/
int Filesys::get_root_size()
{
    return root_size;
}

/*********************************************************
 * returning fat size
*********************************************************/
int Filesys::get_fat_size()
{
    return fat_size;
}

bool Filesys::check_block(string file, int block_number)
{
    int iblock = get_first_block(file);
    while(iblock != 0) {
        if(iblock == block_number) {
            return true;
        }
        iblock = fat[iblock];
    }
    return false;
}

//returns the vector containing the filenames. 
vector<string> Filesys::ls()
{
	vector<string> flist;
	for (int i = 0; i < filename.size(); ++i) {
		if (filename[i] != "xxxxxxxx") {
			flist.push_back(filename[i]);
		}
	}
	return flist;
}

