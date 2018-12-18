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

// std::vector<std::string> read_file(std::string filename) {

// }

//returns chars and percentage of appearance as double
std::vector< std::pair<char, double> > scan(std::string filename) {
	//open file and start reading
	std::vector< std::pair<char, double> > percentages;

	return percentages;
}

/*

	Encode and decode return vector<string> for two reasons:

	* decode will return the huffman tree as well, 
		(do i actually want to do that?)

	* Eventually the vector will contain one block of data apiece for ez writin

*/

std::vector<std::string> encode(std::ifstream& filestream) {
	//scan file to get char values

	//make huffman tree

	//make file called filename + .huff

	//write huffman tree values to file, and EOT char (double newline?)

	//write rest of file

	//--------------right now this just returns a vector<string> of read file
	std::vector<std::string> encoded;
	std::string line;
	while (getline(filestream, line)) {
		encoded.push_back(line);
	}
	filestream.close();
	return encoded;
}

std::vector<std::string> decode(std::ifstream& filestream) {
	//read tree, put that into separate memory from rest of file

	//re-create tree using read character values

	//make new file called filename.substr(filename.size()-5)

	//decode compressed file and write to new file
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
		output.open(filename + ".huff");
	}

	//deletes original file. is it better to overwrite original file?
	std::remove(argv[1]);

	//write data to file
	for (const std::string& line : file_data) {
		output << line;
	}
	output.close();

	//calculate space saved

	return 0;
}