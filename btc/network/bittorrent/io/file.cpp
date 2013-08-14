#include "file.h"

using namespace network::bittorrent::io;

file::file(std::wstring path, int64_t file_length, long piece_size){
	_path = path;
	_downloaded = 0;
	_file_length = file_length;
	_piece_size = piece_size;

	char buffer[2] = {" "};

	try{
		boost::filesystem::path _directory(path);
		_directory.remove_filename();
		boost::filesystem::create_directories(_directory);

		_file.open(_path, std::ios::trunc | std::ios::binary | std::ios::out);

		if(_file.is_open()){
			_file.seekp(_file_length - sizeof(buffer), std::ios::beg);
            _file.write(buffer, sizeof(buffer));                                    //specify file length
			_file.close();
		}
	}catch(std::exception e){
		if(_file.is_open()){
			_file.close();
		}
	}
}

file::~file(){
}

int64_t file::left(){
	return _file_length - _downloaded;
}

int64_t file::downloaded(){
	return _downloaded;
}

int64_t file::length(){
	return _file_length;
}

/*void file::write(std::vector<char>& data, long piece_number){
	//boost::mutex::scoped_lock lock(_guard);

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
}*/

void file::write(std::vector<char>& data, int64_t absolute_position){
	//boost::mutex::scoped_lock lock(_guard);

    if(std::find(_loaded.begin(), _loaded.end(),
                 absolute_position) != _loaded.end() || !data.size()){
		return;
	}

	char* data_ptr = new char[data.size() + 1];
	memset(data_ptr, 0, data.size() + 1);
	std::copy(data.begin(), data.end(), data_ptr);

	try{
		_file.open(_path, std::ios::binary | std::ios::out| std::ios::in);

		if(_file.is_open()){
			_file.seekp(absolute_position, std::ios::beg);
			_file.write(data_ptr, data.size());
			_file.close();

			_downloaded += (int64_t)data.size();
			_loaded.push_back(absolute_position);
		}
		
        delete[] data_ptr;
	}catch(std::exception e){
		delete[] data_ptr;

		if(_file.is_open()){
			_file.close();
		}
	}
}
