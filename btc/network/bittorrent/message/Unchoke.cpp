#include "Unchoke.h"

using namespace network::bittorrent::message;

Unchoke::Unchoke(){
	data = std::vector<char>(3, 0);
	data.insert(data.end(), (char)0x1);
	data.insert(data.end(), (char)0x1);
}

Unchoke Unchoke::create(){
	return Unchoke();
}

std::vector<char> Unchoke::message(){
	return data;
}

bool Unchoke::is(std::vector<char>& input){
	return ((input[3] == 0x1) && (input[4] == 0x1));
}

type Unchoke::what(){
	return _unchoke;
}
