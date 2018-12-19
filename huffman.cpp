#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

/*
	This program takes in a filename, and encodes it Huffman style
	The filename is then output with filenamename.extension.huff

	The program automatically detects if the filename is already encoded

	TODO: make man page
	Get this working then improve file read/write practices

	Try encoding in hex or oct and use B/B+ trees?

	Try buffered reading to use less memory? (read in ez cachable blocks)
*/

//returns chars and percentage of appearance as double
std::unordered_map<char, double>* get_percentages(std::ifstream& filestream) {
	//init occurences map ptr
	auto occurences = new std::unordered_map<char, double>();
	long total_chars = 0;
	
	//read through file and gather data for percentages
	std::unordered_map<char, double>::iterator it;
	while (!filestream.eof()) {
		char cur = filestream.get();
		it = occurences->find(cur);
		if (it == occurences->end()) {
			occurences->emplace(cur, 1.0);
		} else {
			it->second += 1.0;
		}
		total_chars++;
	}

	//modify occurences to hold averages and return
	total_chars--; //there is a char at the beginning of every file we skip
	for (it = occurences->begin(); it != occurences->end(); it++) {
		it->second /= total_chars;
	}

	return occurences;
}

/*

	Encode and decode return vector<string> for two reasons:

	* decode will return the huffman tree as well, 
		(do i actually want to do that?)

	* Eventually the vector will contain one block of data apiece for ez writin

*/

std::vector<std::string> encode(std::ifstream& filestream) {
	//scan file to get char values
	//currently the file is read through once to get percentages,
	// then again to compress. Is it possible to do it all at once? I doubt it.
	//map ptr
	auto occurences = get_percentages(filestream);

	//make huffman tree

	//write huffman tree values to file, and EOT char (double newline?)

	//write rest of file

	//--------------right now this just returns a vector<string> of read file
	
	//reset filestream to beginning
	filestream.seekg(0, filestream.beg); //not working??
	//http://www.cplusplus.com/reference/istream/istream/seekg/
	//TODO: read link. get fstream reset working
	//	it also has a good example of how to read and store a file

	std::vector<std::string> encoded;
	std::string line;
	while (getline(filestream, line)) {
		std::cout << line << std::endl;
		encoded.push_back(line);
	}
	filestream.close();

	//printing percentages map too
	for (auto it = occurences->begin()++; it != occurences->end(); it++) {
		if (it->first == ' ')
			std::cout << "space" << " : " << it->second * 100.0 << "%\n";
		else if (it->first == '\n')
			std::cout << "newline" << " : " << it->second * 100.0 << "%\n";
		else
			std::cout << it->first << " : " << it->second * 100.0 << "%\n";
	}
	std::cout << std::endl;
	delete occurences; //put this in the right place if you move occurences

	return encoded;
}

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
		output.open(filename + ".huff");
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