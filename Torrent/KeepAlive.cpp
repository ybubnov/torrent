#include "KeepAlive.h"

using namespace network::bittorrent::message;

KeepAlive::KeepAlive(){
	data = std::vector<char>(4, 0);
}

KeepAlive KeepAlive::create(){
	return KeepAlive();
}

std::vector<char> KeepAlive::message(){
	return data;
}

type KeepAlive::what(){
	return _keep_alive;
}