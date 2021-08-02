
#include "IO/File.h"

#include <iostream>
#include <fstream>

File::File(const Str& path) {

	std::ifstream infile(path.str);

	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	buff = new char[length];

	//read file
	infile.read(buff, length);
	size = length;
}
