#include "PeerWireProtocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

PeerWireProtocol::PeerWireProtocol(peer_wire::peer stranger, PieceControl* piece_control, 
	std::vector<char> info_hash, std::vector<char> peer_id) : _peer(stranger){
	
	_tcp = 0;

	_piece_control = piece_control;
	_last_message = message::_undefined;

	_info_hash = info_hash;
	_peer_id = peer_id;

	_conversation = 0;
}

PeerWireProtocol::PeerWireProtocol(peer_wire::peer stranger, PieceControl* piece_control, 
	std::string info_hash, std::vector<char> peer_id) : _peer(stranger){
	
	_tcp = 0;

	_piece_control = piece_control;
	_last_message = message::_undefined;


	std::stringstream hash_flow;
	std::vector<char> hash;

	hash_flow << info_hash;

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
	_peer_id = peer_id;

	_conversation = 0;
}

PeerWireProtocol::PeerWireProtocol(peer_wire::peer stranger, PieceControl* piece_control, 
	std::string info_hash, std::string peer_id) : _peer(stranger){
	
	_tcp = 0;

	_piece_control = piece_control;
	_last_message = message::_undefined;


	std::stringstream hash_flow;
	std::vector<char> hash;
	std::vector<char> id(peer_id.begin(), peer_id.end());

	hash_flow << info_hash;

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

	_conversation = 0;
}

PeerWireProtocol::~PeerWireProtocol(){
	_tcp->stop();
	delete _tcp;
}

void PeerWireProtocol::response_handle(std::istream* response){
	std::string string;

	std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;

	while(response && !response->eof()){
		*response >> string;
		std::cout << string;
	}
	std::cout << std::endl << "done" << std::endl;
	std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
	conversation();
}

void PeerWireProtocol::request(){
	std::cout << "REQUEST" << std::endl;

	message::interface& request = message::request::create(0xa, 0x0, 0x4000);

	try{
		_last_message = message::_request;
		_tcp->send(this, request.message());
	}catch(std::exception e){
		std::cout << "BAD REQUEST" << std::endl;
	}
}

void PeerWireProtocol::conversation(){
	try{
		if(!_tcp){
			_tcp = new network::tcp::protocol(_peer.ip(), _peer.port());
		}
	}catch(std::exception e){
		std::cout << "BAD CONNECTION DETECTED" << std::endl;
		return;
	}

	switch(_last_message){
		case message::_handshake:{
			std::cout << "INTERESTED" << std::endl;

			message::interface& intereseted = message::interested::create();

			try{
				//_last_message = message::_interested;
				//_tcp->send(intereseted.message());

				request();
			}catch(std::exception e){
				std::cout << "BAD INTERESTED" << std::endl;
			}

			break;
		}
		case message::_interested:{
			request();
			break;
		}
		case message::_request:{
			//request();
			std::cout << "REQUEST SUCCESSFULL" << std::endl;
			break;
		}
		case message::_undefined:{
			std::cout << "HANDSHAKE" << std::endl;

			message::interface& handshake = message::handshake::create(_info_hash, _peer_id);

			std::vector<char> mes = handshake.message();
			std::string mess(mes.begin(), mes.end());
			std::cout << mess << std::endl;

			try{
				_last_message = message::_handshake;
				_tcp->send(this, handshake.message());
			}catch(std::exception e){
				std::cout << "BAD HANDSHAKE" << std::endl;
				std::cout << _peer.ip() << ":" << _peer.port() << std::endl;
			}
			break;
		}
		default:
			break;
	}
}

void PeerWireProtocol::yeild(){
	if(!_conversation){
		_conversation = new boost::thread(boost::bind(&PeerWireProtocol::conversation, this));
	}
	_conversation->yield();
}