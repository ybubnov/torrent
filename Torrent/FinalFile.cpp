#include "FinalFile.h"

FinalFile::FinalFile(std::string path, long piece_size){
	_path = path;
	_piece_size = piece_size;
}

void FinalFile::write(std::vector<char> data, long piece_number){
	boost::mutex::scoped_lock lock(_guard);

	try{
		//file...
	}catch(std::exception e){
	}

	//actions
}