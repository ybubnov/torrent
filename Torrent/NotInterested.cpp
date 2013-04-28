#include "NotInterested.h"

using namespace network::bittorrent::message;

NotInterested::NotInterested(){
	data = std::vector<char>(3, 0);
	data.insert(data.end(), (char)0x1);
	data.insert(data.end(), (char)0x3);
}

NotInterested NotInterested::create(){
	return NotInterested();
}

std::vector<char> NotInterested::message(){
	return data;
}

type NotInterested::what(){
	return _not_interested;
}