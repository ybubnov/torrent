#include "Choke.h"

using namespace network::bittorrent::message;

Choke::Choke(){
	data = std::vector<char>(3, 0);
	data.insert(data.end(), (char)0x1);
	data.insert(data.end(), (char)0x0);
}

Choke Choke::create(){
	return Choke();
}

std::vector<char> Choke::message(){
	return data;
}

type Choke::what(){
	return _choke;
}