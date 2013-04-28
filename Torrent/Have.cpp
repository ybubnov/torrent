#include "Have.h"

using namespace network::bittorrent::message;

Have::Have(long index){
	data = std::vector<char>(3, 0);

	data.insert(data.end(), 0x5);
	data.insert(data.end(), 0x4);

	data.insert(data.end(), (char)((index & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((index & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((index & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((index & 0x000000ff)));

}

Have Have::create(long index){
	return Have(index);
}

std::vector<char> Have::message(){
	return data;
}

type Have::what(){
	return _have;
}