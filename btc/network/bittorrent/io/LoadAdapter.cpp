#include "LoadAdapter.h"

using namespace network::bittorrent::io;

LoadAdapter::LoadAdapter(network::bittorrent::io::notifiable* subscriber, std::list<FinalFile*>& file_list, long piece_length){
	_file_list = std::vector<FinalFile*>(file_list.begin(), file_list.end());
	_piece_length = piece_length;
	_subscriber = subscriber;

	int64_t file_length;
	int64_t count;

	_block_list.push_back(0);
	_length_list.push_back(0);

	for(unsigned int i = 0; i < _file_list.size(); i++){
		file_length = _file_list[i]->length();

		count = file_length / (int64_t)_piece_length;

		if(file_length % (int64_t)_piece_length){
			count++;
		}

		_length_list.push_back(file_length + _length_list[i]);
		_block_list.push_back((long)count + _block_list[i]);
	}

	_loaded = 0;
	_total_length = _length_list[_length_list.size() - 1];

	for(unsigned int i = 0; i < _file_list.size(); i++){
		_loaded += _file_list[i]->downloaded();
	}
}

LoadAdapter::~LoadAdapter(){
}

int64_t LoadAdapter::downloaded(){
	return _loaded;
}

int64_t LoadAdapter::left(){
	return _total_length - _loaded;
}

void LoadAdapter::write(std::vector<char>& data, long piece_number){
	double progress;
	double loaded;
	int64_t absolute_position = (int64_t)piece_number * _piece_length;

	boost::mutex::scoped_lock lock(_guard);

	for(unsigned int i = 0; i < _block_list.size() - 1; i++){
		if(_block_list[i] <= piece_number && _block_list[i + 1] > piece_number){

			if(_file_list[i]->left() <= 0){
				return;
			}else if(data.size() > _file_list[i]->left()){
				std::vector<char> low_data(data.begin(), data.begin() + (long)_file_list[i]->left());
				std::vector<char> high_data(data.begin() + (long)_file_list[i]->left(), data.end());

				if(i + 1 < _file_list.size()){
					_file_list[i]->write(low_data, absolute_position - _length_list[i]);
					_file_list[i + 1]->write(high_data, (int64_t)0);
				}else{
					return;
				}

			}else{
				if(i < _file_list.size()){
					_file_list[i]->write(data, absolute_position - _length_list[i]);
				}else{
					return;
				}
			}

			_loaded = 0;
			for(unsigned int i = 0; i < _file_list.size(); i++){
				_loaded += _file_list[i]->downloaded();
			}

			try{
				if(_subscriber){
					loaded = (double)downloaded();

					if(_length_list[_length_list.size() - 1] != 0){
						progress = loaded / (double)_length_list[_length_list.size() - 1];
					}else{
						progress = 0;
					}
					_subscriber->notify(progress * 100);
				}
			}catch(std::exception){
				_subscriber->notify(0);
			}

			break;
		}
	}
}
