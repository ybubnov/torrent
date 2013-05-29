#include "Handshake.h"

#include <sstream>
#include <iomanip>

using namespace network::bittorrent::message;

Handshake::Handshake(std::vector<char> info_hash, std::vector<char> peer_id){
	std::vector<char> reserved(8, 0);
	std::string protocol = "BitTorrent protocol";
	int pstrlen = protocol.size();

	data.push_back(static_cast<char>(pstrlen));
	data.insert(data.end(), protocol.begin(), protocol.end());
	data.insert(data.end(), reserved.begin(), reserved.end());
	data.insert(data.end(), info_hash.begin(), info_hash.end());
	data.insert(data.end(), peer_id.begin(), peer_id.end());
}

Handshake Handshake::create(std::vector<char> info_hash, std::vector<char> peer_id){
	return Handshake(info_hash, peer_id);
}

Handshake Handshake::create(std::string info_hash, std::string peer_id){
	std::stringstream hash_flow;
	std::stringstream id_flow;

	std::vector<char> hash;
	std::vector<char> id/*(peer_id.begin(), peer_id.end())*/;

	hash_flow << info_hash;
	id_flow << peer_id;

	char high_digit;
	char low_digit;
	int value;

	while(hash_flow >> high_digit && hash_flow >> low_digit){
		std::stringstream buffer_flow;

		buffer_flow << high_digit << low_digit;
		buffer_flow >> std::hex >> value;

		value &= 0xff;

		hash.push_back((char)value);
	}

	while(id_flow >> high_digit && id_flow >> low_digit){
		std::stringstream buffer_flow;

		buffer_flow << high_digit << low_digit;
		buffer_flow >> std::hex >> value;

		value &= 0xff;

		id.push_back((char)value);
	}

	return Handshake(hash, id);
}

std::vector<char> Handshake::message(){
	return data;
}

bool Handshake::is(std::vector<char>& input){
	if(input.size() < 19){
		return false;
	}

	long pstrlen = (long)*input.begin();
	std::string protocol(input.begin() + 1, input.begin() + pstrlen + 1);

	return !protocol.compare("BitTorrent protocol");
}

type Handshake::what(){
	return _handshake;
}

std::vector<char> Handshake::crop(std::vector<char>& incoming){
	if(incoming.size() > 68){
		return std::vector<char>(incoming.begin() + 68, incoming.end());
	}else{
		throw std::exception();
	}
}
