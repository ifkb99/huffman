#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include "tree/btree.h"

/*
	This program takes in a filename, and encodes it Huffman style
	The filename is then output with filenamename.extension.huff

	The program automatically detects if the filename is already encoded

	TODO: make man page
	Get this working then improve file read/write practices

	Try encoding in hex or oct and use B/B+ trees?

	Try buffered reading to use less memory? (read in ez cachable blocks)

	try using char array instead of vector as seen below:
	http://www.cplusplus.com/reference/istream/istream/seekg/
		it has a good example of how to read and store a file
*/

//lazy lt func for sorting vector
const bool char_less(std::pair<char, long> a, std::pair<char, long> b) {
	return a.second < b.second;
}

//returns chars and percentage of appearances as double
std::vector< std::pair<char, long> > get_percentages(char* file, size_t length) {
	//init occurrences map
	std::unordered_map<char, long> occurrences;
	
	//read through file and gather data for percentages
	std::unordered_map<char, long>::iterator it;
	
	for (size_t i=0; i<length; i++) {
		it = occurrences->find(file[i]);
		if (it != occurrences->end()) {
			//char is in map, increment occurrences
			it->second += 1;
		} else {
			//char not in map, place in map
			occurrences->emplace(file[i], 1);
		}
	}

	// //modify occurrences to hold averages and return
	// for (it = occurrences->begin(); it != occurrences->end(); it++) {
	// 	it->second /= length;
	// }
	std::vector< std::pair<char, long> > occurrence_list;
	occurrence_list.reserve(occurrences.size());
	for (const auto& item : occurrences) {
		occurrence_list.push_back(item);
	}
	std::sort(occurrence_list.begin(), occurrence_list.end(), char_less());

	return occurrence_list;
}

/*

	Encode and decode return vector<string> for two reasons:

	* decode will return the huffman tree as well, 
		(do i actually want to do that?)

	* Eventually the vector will contain one block of data apiece for ez writin

*/

//remember to write as binary when writing file
std::vector<std::string> encode(std::ifstream& fs) {
	//get len of file
	fs.seekg(0, fs.end);
	size_t length = fs.tellg()-1;
	fs.seekg(1, fs.beg); //skip first char

	//load file into memory
	char* file = new char[length];

	for (uint i=0; i<length; i++)
		file[i] = fs.get();

	//scan file to get char values
	//vector of char/amount pairs in order
	auto occurrences = get_percentages(file, length);

	//make huffman tree
	BTree huff_tree;
	for (const auto* chr_pair : occurrences)
		huff_tree.insert(chr_pair);	

	//write huffman tree values to file, and EOT char (double newline?)
	//first 8 bits are int of how many bits the huff tree is (can it be 4 bits?)
	

	//write rest of file

	fs.close();

	//printing percentages map too
	//first char of file is begin byte, so it's skipped
	for (auto it = ++occurrences->begin(); it != occurrences->end(); it++) {
		if (it->first == ' ')
			std::cout << "space";
		else if (it->first == '\n')
			std::cout << "newline";
		else
			std::cout << it->first;
		std::cout << " : " << it->second * 100.0 << "%\n";
	}
	std::cout << std::endl;
	delete occurrences; //put this in the right place if you move occurrences

	return encoded;
}

//remember to read as binary
std::vector<std::string> decode(std::ifstream& filestream) {
	//read tree, put that into separate memory from rest of file

	//re-create tree using read character values

	//make new file called filename.substr(filename.size()-5)

	//decode compressed file and write to new file

	//----------------test output atm
	std::vector<std::string> decoded;
	std::string line;
	while (getline(filestream, line)) {
		decoded.push_back(line);
	}
	filestream.close();
	return decoded;
}

int main(int argc, const char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] 
			<< " <filename to encode/decode>" << std::endl;
		return 1;
	}

	//get filename extension split
	std::string filename(argv[1]);
	int dot = filename.rfind('.');
	std::string extension;
	if (dot != std::string::npos)
	 	extension = filename.substr(dot);

	//determine if file is to be encoded or decoded
	std::ifstream input(filename);
	std::ofstream output;
	std::vector<std::string> file_data;
	if (extension == ".huff") { //is already encoded
		file_data = decode(input);
		output.open(filename.substr(0, filename.size()-5)); //removes .huff
	} else { //isn't encoded
		file_data = encode(input);
		output.open(filename + ".huff", std::ios::binary); //write as binary
	}

	//deletes original file. is it better to overwrite original file?
	std::remove(argv[1]);

	//write data to file
	for (const std::string& line : file_data) {
		output << line << '\n'; //newline is stripped rn
	}
	output.close();

	//calculate space saved

	return 0;
}