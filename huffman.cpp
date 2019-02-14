#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include "tree/btree.h"

#define BUFFER_SIZE 4096 //4kb

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
void get_percentages(char* file, size_t length,
						std::unordered_map<char, long>& occurrences) {
	
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
	// std::vector< std::pair<char, long> > occurrence_list;
	// occurrence_list.reserve(occurrences.size());
	// for (const auto& item : occurrences) {
	// 	occurrence_list.push_back(item);
	// }
	// std::sort(occurrence_list.begin(), occurrence_list.end(), char_less());

	// return occurrence_list;
}

/*

	Encode and decode return vector<string> for two reasons:

	* decode will return the huffman tree as well, 
		(do i actually want to do that?)

	* Eventually the vector will contain one block of data apiece for ez writin

*/

//https://stackoverflow.com/questions/20005784/saving-a-binary-tree-to-a-file
void preorder_make_string(Node<char, long>& node, std::string& serialized) {
	preorder_make_string(node.left);
	if (node == nullptr) {
		serialized += "# ";
	} else {
		serialized += node.key() << ":" << node.value() << " ";
	}
	preorder_make_string(node.right);
}

void serialize_huffman(BTree& tree, std::string& huff_str) {
	//char:occurences + nulls + spaces = 6 + leaves
	huff_str.reserve(tree.size() * (6 + (2 * tree.leaves())));
	preorder_make_string(tree.root(), huff_str);
}

//remember to write as binary when writing file
BTree encode(std::ifstream& fs, BTree& huff_tree) {
	//get len of file
	fs.seekg(0, fs.end);
	size_t length = fs.tellg();
	fs.seekg(0, fs.beg);

	//init percentages map
	std::unordered_map<char, long> occurrences;

	//load file into buffer, get char occurences
	char buffer[BUFFER_SIZE];
	int chunks = length / BUFFER_SIZE;
	int rmdr = length % BUFFER_SIZE;
	for (int i=0; i<chunks; i++) {
		//put data in buffer
		fs.get(buffer, BUFFER_SIZE);
		get_percentages(buffer, BUFFER_SIZE, occurrences);
	}

	//if leftover then read
	if (rmdr) {
		fs.get(buffer, rmdr);
		get_percentages(buffer, rmdr, occurrences);
	}

	//close stream
	fs.close();

	//fill huffman tree
	for (const auto* chr_pair : occurrences) //make node from occurences
		huff_tree.insert(chr_pair);

	return huff_tree;
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
	if (extension == ".huff") { //is already encoded
		std::vector<std::string> file_data = decode(input);
		output.open(filename.substr(0, filename.size()-5)); //removes .huff

		//write data to file
		for (const std::string& line : file_data) {
			output << line << '\n'; //newline is stripped rn //is it really tho
		}
	} else { //isn't encoded
		BTree huff_tree;
		encode(input, huff_tree);
		output.open(filename + ".huff");//, std::ios::binary); //write as binary

		//turn huffman to string
		std::string huff_str;
		serialize_huffman(huff_tree, huff_str);

		//prepend amount of chars in tree rep into str
		huff_str = huff_str.size() + " " + huff_str;

		//write huffstr to file

		//http://www.cplusplus.com/reference/ios/ios_base/flags/
		//change flags to write in binary

		//write remainder of file in binary using huffman tree.find
		//I think I need a new func to return the path

		
	}

	//deletes original file. is it better to overwrite original file?
	//std::remove(argv[1]);

	
	output.close();

	//calculate space saved

	return 0;
}