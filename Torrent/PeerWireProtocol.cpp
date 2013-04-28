#include "PeerWireProtocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

PeerWireProtocol::PeerWireProtocol(peer_wire::peer stranger, PieceControl* piece_control, 
	std::vector<char> info_hash, std::vector<char> peer_id) : 

	_peer(stranger),
	_conversation(boost::bind(&PeerWireProtocol::conversation, this)){
	
	_piece_control = piece_control;

	_tcp = new network::tcp::protocol(stranger.ip(), stranger.port());
	_last_message = message::_undefined;

	_info_hash = info_hash;
	_peer_id = peer_id;
}

PeerWireProtocol::PeerWireProtocol(peer_wire::peer stranger, PieceControl* piece_control, 
	std::string info_hash, std::string peer_id) : 

	_peer(stranger),
	_conversation(boost::bind(&PeerWireProtocol::conversation, this)){
	
	_piece_control = piece_control;

	_tcp = new network::tcp::protocol(stranger.ip(), stranger.port());
	_last_message = message::_undefined;


	std::stringstream hash_flow;
	std::stringstream id_flow;

	std::vector<char> hash;
	std::vector<char> id(peer_id.begin(), peer_id.end());

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

	_info_hash = hash;
	_peer_id = id;
}

PeerWireProtocol::~PeerWireProtocol(){
	_tcp->stop();
	delete _tcp;
}

void PeerWireProtocol::response_handle(std::istream* response){
	std::stringstream string_flow;
	std::string string;

	while(response && !response->eof()){
		*response >> string;
		std::cout << " " << string;
	}
	std::cout << "done" << std::endl;

	conversation();
}

void PeerWireProtocol::conversation(){
	switch(_last_message){
		case message::_handshake:{
			message::interface& intereseted = message::interested::create();
			_tcp->send(this, intereseted.message());
			_last_message = message::_interested;

			break;
		}
		case message::_interested:{
			message::interface& request = message::request::create(0, 0, 0x1800);
			_tcp->send(this, request.message());
			_last_message = message::_request;

			break;
		}
		case message::_request:{

			break;
		}
		case message::_undefined:{
			std::cout << "HANDSHAKE" << std::endl;

			message::interface& handshake = message::handshake::create(_info_hash, _peer_id);

			std::vector<char> mes = handshake.message();
			std::string mess(mes.begin(), mes.end());
			std::cout << mess << std::endl;

			_tcp->send(this, handshake.message());
			_last_message = message::_handshake;

			break;
		}
		default:
			break;
	}
}

void PeerWireProtocol::yeild(){
	_conversation.yield();
}