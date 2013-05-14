#include "PeerWireProtocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

const long PeerWireProtocol::block_length = 0x4000;

void PeerWireProtocol::initialize(){
	_begin = 0;
	_tcp = 0;
	_conversation = 0;
	_last_message = message::_undefined;

	if(_piece_control->piece_length() < PeerWireProtocol::block_length){
		_piece_length = _piece_control->piece_length();
	}else{
		_piece_length = PeerWireProtocol::block_length;
	}
}

PeerWireProtocol::PeerWireProtocol(
	peer_wire::peer stranger, 
	PieceControl* piece_control, 
	FinalFile* file,
	std::vector<char> info_hash, 
	std::vector<char> peer_id) : _peer(stranger){

	_piece_control = piece_control;
	_info_hash = info_hash;
	_peer_id = peer_id;
	_file = file;

	initialize();
}

PeerWireProtocol::PeerWireProtocol(
	peer_wire::peer stranger, 
	PieceControl* piece_control, 
	FinalFile* file,
	std::string info_hash, 
	std::vector<char> peer_id) : _peer(stranger){
	
	_piece_control = piece_control;

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
	_file = file;

	initialize();
}

PeerWireProtocol::PeerWireProtocol(
	peer_wire::peer stranger, 
	PieceControl* piece_control, 
	FinalFile* file,
	std::string info_hash, 
	std::string peer_id) : _peer(stranger){

	_piece_control = piece_control;

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

	initialize();
}

PeerWireProtocol::~PeerWireProtocol(){
	_tcp->stop();
	delete _tcp;
	delete _current_piece;
	delete _conversation;
}

void PeerWireProtocol::response_handle(std::istream* response){
	char buf[0x7fff];
	memset(buf, 0, sizeof(buf));

	std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;

	response->read(buf, _piece_length);

	std::cout << buf << std::endl;
	std::cout << "done" << std::endl;
	std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;

	std::vector<char> incoming(buf, buf + sizeof(buf));

	if(message::handshake::is(incoming)){
		_last_message = message::_handshake;
	}else if(message::unchoke::is(incoming)){
		_last_message = message::_unchoke;
	}
}

void PeerWireProtocol::shift(){

	if(_begin + _piece_length < _piece_control->piece_length()){
		_begin += _piece_length;

		if(_begin + _piece_length > _piece_control->piece_length()){
			_piece_length = _piece_control->piece_length() - _begin;
		}

	}else{
		_file->write(_buffer, _current_piece->index());
		_piece_control->downloaded(*_current_piece);

		_begin = 0;
	
		try{
			_current_piece = _piece_control->next_piece();
		}catch(std::exception e){
			_last_message = message::_undefined;
		}

		if(_piece_control->piece_length() < PeerWireProtocol::block_length){
			_piece_length = _piece_control->piece_length();
		}else{
			_piece_length = PeerWireProtocol::block_length;
		}
	}
}

void PeerWireProtocol::request(){
	long piece_begin = _begin;
	long piece_index = _current_piece->index();
	long piece_length = _piece_length;


	std::cout << "REQUEST" << std::endl;
	std::cout << "BEGIN = " << piece_begin << " INDEX = " << piece_index << " LENGTH = " << piece_length << std::endl;

	message::interface& request = message::request::create(piece_index, piece_begin, piece_length);

	try{
		_last_message = message::_request;
		_tcp->send(this, request.message());
	}catch(std::exception e){
		std::cout << "BAD REQUEST" << std::endl;
		return;
	}
}

bool PeerWireProtocol::handshake(){
	std::cout << "HANDSHAKE" << std::endl;

	message::interface& handshake = message::handshake::create(_info_hash, _peer_id);

	try{
		_tcp->send(this, handshake.message());
	}catch(std::exception e){
		std::cout << "BAD HANDSHAKE" << std::endl;
		return false;
	}

	return _last_message == message::_handshake;
}

bool PeerWireProtocol::interested(){
	std::cout << "INTERESTED" << std::endl;

	message::interface& intereseted = message::interested::create();

	try{
		_tcp->send(this, intereseted.message());
	}catch(std::exception e){
		std::cout << "BAD INTERESTED" << std::endl;
		return false;
	}

	return _last_message == message::_unchoke;
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

	if(handshake()){
		if(interested()){
			_current_piece = _piece_control->next_piece();

			request();
		}
	}
}

void PeerWireProtocol::yeild(){
	if(!_conversation){
		_conversation = new boost::thread(boost::bind(&PeerWireProtocol::conversation, this));
	}
	_conversation->yield();
}