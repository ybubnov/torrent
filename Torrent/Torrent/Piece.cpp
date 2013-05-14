#include "Piece.h"
#include <sstream>

using namespace network::bittorrent::message;

Piece::Piece(long index, long begin, std::vector<char> bulk){
	data = std::vector<char>(3, 0);

	data.insert(data.end(), 0x9 + bulk.size());
	data.insert(data.end(), 0x7);

	data.insert(data.end(), (char)((index & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((index & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((index & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((index & 0x000000ff)));

	data.insert(data.end(), (char)((begin & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((begin & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((begin & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((begin & 0x000000ff)));

	data.insert(data.end(), bulk.begin(), bulk.end());
}

Piece Piece::create(long index, long begin, std::vector<char> bulk){
	return Piece(index, begin, bulk);
}

std::vector<char> Piece::message(){
	return data;
}

type Piece::what(){
	return _piece;
}

std::vector<char> Piece::block(std::vector<char> incoming){
	std::stringstream flow;
	std::string size(incoming.begin(), incoming.begin() + 4);
	unsigned long block_size;

	flow << size;
	flow.read((char*)&block_size, sizeof(unsigned long));

	block_size -= 9;

	if(incoming.at(4) == 0x7 && incoming.size() > block_size){
		return std::vector<char>(incoming.begin() + 13, incoming.begin() + block_size);
	}

	throw std::exception();
}