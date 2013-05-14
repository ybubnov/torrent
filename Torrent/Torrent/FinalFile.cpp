#include "FinalFile.h"

FinalFile::FinalFile(std::string path, int64_t file_length, long piece_size){
	_path = path;
	_file_length = file_length;
	_piece_size = piece_size;

	char buffer[2] = {" "};

	try{
		_file.open(_path, std::ios::trunc | std::ios::binary | std::ios::out);

		if(_file.is_open()){
			_file.seekp(_file_length - sizeof(buffer), std::ios::beg);
			_file.write(buffer, sizeof(buffer));
			_file.close();
		}
	}catch(std::exception e){
		if(_file.is_open()){
			_file.close();
		}
	}
}

void FinalFile::write(std::vector<char> data, long piece_number){
	boost::mutex::scoped_lock lock(_guard);

	char* data_ptr = &data[0];

	try{
		_file.open(_path, std::ios::binary | std::ios::out);

		if(_file.is_open()){
			_file.seekp(piece_number * _piece_size, std::ios::beg);
			_file.write(data_ptr, data.size());
			_file.close();
		}

	}catch(std::exception e){
		if(_file.is_open()){
			_file.close();
		}
	}

	_file.close();
}