#include "parser.h"

using namespace network::bittorrent::peer_wire;

parser::parser(std::vector<char> peers){
	_peers_vector = peers;
}

int parser::num(char value){
	return(int)(value & 0xff);
}

std::list<peer> parser::peers(){
	if(_peers_list.size()){
		return _peers_list;
	}

	int port_number;

	try{
		for(std::vector<char>::iterator it = _peers_vector.begin(); it != _peers_vector.end(); it++){
			std::stringstream ip;
			std::stringstream port;
			std::stringstream integer_port;

            ip << num(*it++) << ".";                                                //first four bytes as ip
			ip << num(*it++) << ".";
			ip << num(*it++) << ".";
			ip << num(*it++);

            port << std::hex << num(*it++);                                         //second two bytes as port
			port << std::hex << num(*it);

			port >> std::hex >> port_number;
			integer_port << port_number;

            _peers_list.push_back(peer(ip.str(), integer_port.str()));
		}
	}catch(std::bad_exception e){

	}

	return _peers_list;
}
