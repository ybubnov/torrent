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

bool Piece::is(std::vector<char>& incoming){
	if(incoming.size() < 5){
		return false;
	}

	return incoming[4] == 0x7;
}

type Piece::what(){
	return _piece;
}

std::vector<char> Piece::block(std::vector<char>& incoming){
	unsigned long block_size = 0;

	if(incoming.size() < 13){
		throw std::exception();
	}

	block_size |= (((unsigned long)incoming[0] & 0xff) << 0x18);
	block_size |= (((unsigned long)incoming[1] & 0xff) << 0x10);
	block_size |= (((unsigned long)incoming[2] & 0xff) << 0x8);
	block_size |= (((unsigned long)incoming[3] & 0xff));

	block_size -= 9;

	if(incoming[4] == 0x7 && incoming.size() > block_size){
		return std::vector<char>(incoming.begin() + 13, incoming.begin() + 13 + block_size);
	}

	throw std::exception();
}

std::vector<char> Piece::block(std::vector<char>& incoming, long length){
	if(incoming.size() < (unsigned long)length){
		throw std::exception();
	}

	if(incoming[4] == 0x7){
		return std::vector<char>(incoming.begin() + 13, incoming.begin() + length);
	}

	throw std::exception();
}