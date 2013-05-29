#include "Peer.h"

using namespace network::bittorrent::peer_wire;

Peer::Peer(std::string ip, std::string port){
	_ip = ip;
	_port = port;
}

std::string Peer::ip(){
	return _ip;
}

std::string Peer::port(){
	return _port;
}
