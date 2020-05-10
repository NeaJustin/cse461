#include "Table.h"

Table::Table(std::string diskname, int block_size, int number_of_blocks, std::string flat_file, std::string index_file)
{
	this->flatfile = flat_file;
	this->indexfile = index_file;
	new_file(flatfile);
	new_file(indexfile);
}

int Table::build_table(std::string input_file)
{
	ifstream infile;
	infile.open(input_file.c_str());
	std::string record;
	int count = 0;
	std::getline(infile, record);

	std::vector<std::string> key;
	std::vector<std::string> iblock;

	ostringstream outstream;

	while (infile.good()) {
		std::string primary_key = record.substr(0, 5);
		std::vector<std::string> oblock = block(record, get_block_size());
		int block_id = add_block(flat_file, oblock[0]);
		std::outstream << primary_key << " " << block_id << " ";
		count++;
		if (count == 4) {
			std::vector<std::string> o2_block = block(outstream.str(), get_block_size());
			add_block(index_file, o2_block[0]);
			count = 0;
		}
	}

	return 1;
}

int Table::search(std::string value)
{
	istringstream istream;

	int block_id = get_first_block(index_file);
	
	if (block_id == -1) {
		return 0;
	}

	while (block_id != 0) {
		std::string buff;
		read_block(index_file, block_id, bufF);

		std::string k, b;
		istream.str(buff);

		for (int i = 0; i <= 4; ++i) {
			istream >> k >> b;
			if (k == value) {
				return b;
			}
		}
		block_id = next_block(index_file, block_id);
	}
	return 0;
}
