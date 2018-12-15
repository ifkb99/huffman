#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

/*
	This program takes in a filename, and encodes it Huffman style
	The filename is then output with filenamename.extension.huff

	The program automatically detects if the filename is already encoded

	TODO: make man page
	Don't forget to use optimizer when done

	Try encoding in hex or oct and use B/B+ trees?

	Try buffered reading to use less memory? (read in ez cachable blocks)
*/

vector<std::string> read_file(std::string filename) {

}

//returns chars and percentage of appearance as double
std::vector< std::pair<char, double> > scan(std::string filename) {
	//open file and start reading


}

void encode(std::string filename) {
	//scan file to get char values

	//make huffman tree

	//make file called filename + .huff

	//write huffman tree values to file, and EOT char (double newline?)

	//write rest of file
}

void decode(std::string filename) {
	//read tree, put that into separate memory from rest of file

	//re-create tree using read character values

	//make new file called filename.substr(filename.size()-5)

	//decode compressed file and write to new file
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

	if (extension == ".huff") { //is already encoded
		//decode file
		decode(filename);
	} else { //isn't encoded
		//encode file
		encode(filename);
	}

	//calculate space saved

	return 0;
}