#include "FinalFile.h"

FinalFile::FinalFile(std::string path, int64_t file_length, long piece_size){
	_path = path;
	_downloaded = 0;
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

int64_t FinalFile::left(){
	return _file_length - _downloaded;
}

void FinalFile::write(std::vector<char>& data, long piece_number){
	boost::mutex::scoped_lock lock(_guard);

	if(std::find(_writed.begin(), _writed.end(), piece_number) != _writed.end() || !data.size()){
		return;
	}

	char* data_ptr = new char[data.size() + 1];
	memset(data_ptr, 0, data.size() + 1);
	std::copy(data.begin(), data.end(), data_ptr);

	int64_t seek_length = (int64_t)piece_number * (int64_t)_piece_size;

	try{
		_file.open(_path, std::ios::binary | std::ios::out| std::ios::in);

		if(_file.is_open()){
			_file.seekp(seek_length, std::ios::beg);
			_file.write(data_ptr, data.size());
			_file.close();

			_writed.push_back(piece_number);
			_downloaded += (int64_t)data.size();
		}
		
		delete[] data_ptr;
	}catch(std::exception e){
		delete[] data_ptr;

		if(_file.is_open()){
			_file.close();
		}
	}
}