#include "PeerWireProtocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

const long PeerWireProtocol::block_length = 0x4000;

void PeerWireProtocol::initialize(){
	_offset = 0;
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
	response->read(buf, sizeof(buf));

	std::vector<char> incoming(buf, buf + sizeof(buf));
	_incoming = incoming;

	if(message::handshake::is(incoming)){
		_last_message = message::_handshake;

		/*if(message::bitfield::is(incoming)){
			_have = message::bitfield::field(message::handshake::crop(incoming));
		}*/
	
	}else if(message::unchoke::is(incoming)){
		_last_message = message::_unchoke;
	}else if(message::piece::is(incoming)){
		_last_message = message::_piece;
		std::vector<char> _block;

		try{
			_block = message::piece::block(incoming);
		}catch(std::exception e){
			std::cout << "BLOCK PROBLEM>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

			_last_message = message::_undefined;
			return;
		}

		_buffer.insert(_buffer.end(), _block.begin() + _offset, _block.end());
	}else{
		_last_message = message::_undefined;
	}
}


void PeerWireProtocol::shift(){

	if(_begin + _piece_length < _piece_control->piece_length()){
		_begin += _piece_length;
		_offset = 0;

		if(_begin + _piece_length > _piece_control->piece_length()){
			_offset = _piece_length - (_piece_control->piece_length() - _begin);
			_begin = _piece_control->piece_length() - _piece_length; 
			
		}
	}else{
		_file->write(_buffer, _current_piece->index());

		std::cout << "has written " << _current_piece->index() << std::endl;

		_buffer = std::vector<char>();
		_piece_control->downloaded(_current_piece);
		_begin = 0;
		_offset = 0;
		_current_piece = _piece_control->next_piece();
		
		if(_piece_control->piece_length() < PeerWireProtocol::block_length){
			_piece_length = _piece_control->piece_length();
		}else{
			_piece_length = PeerWireProtocol::block_length;
		}
	}
}


bool PeerWireProtocol::refresh(){
	long previous_index = _current_piece->index();

	_buffer = std::vector<char>();

	_begin = 0;
	_offset = 0;

	try{
		_current_piece = _piece_control->next_piece();

		boost::this_thread::sleep(boost::posix_time::milliseconds(100));

		if(previous_index == _current_piece->index() && _piece_control->left() == 1){
			return false;
		}
	}catch(std::exception e){
		return false;
	}
		
	if(_piece_control->piece_length() < PeerWireProtocol::block_length){
		_piece_length = _piece_control->piece_length();
	}else{
		_piece_length = PeerWireProtocol::block_length;
	}

	return true;
}

bool PeerWireProtocol::request(){
	long piece_begin;
	long piece_index;
	long piece_length;


	try{
		piece_begin = _begin;
		piece_index = _current_piece->index();

		if(_piece_control->left() == 1){
			piece_length = (long)_file->left() - _buffer.size();
		}else{
			piece_length = _piece_length;
		}

		message::interface& request_message = message::request::create(piece_index, piece_begin, piece_length);

		_last_message = message::_undefined;
		_tcp->send(this, request_message.message(), piece_length + 0xd);

		if(_last_message != message::_piece){
			throw std::exception();
		}

		shift();
	}catch(boost::system::system_error e){
		std::cout << "begin = " << piece_begin << " index = " << piece_index << " length = " << piece_length << std::endl;
		std::cout << "FAILURE" << std::endl;
		return false;
	}catch(std::exception e){
		std::cout << "BAD REQUEST" << std::endl;

		/*long _current_index = _current_piece->index();

		message::interface& handshake_message = message::handshake::create(_info_hash, _peer_id);
		_tcp->send(this, handshake_message.message());*/

		if(_piece_control->left()){
			return refresh();
		}

		return false;
	}

	return true;
}

bool PeerWireProtocol::handshake(){
	std::cout << "handshake" << std::endl;

	try{
		message::interface& handshake_message = message::handshake::create(_info_hash, _peer_id);
		_tcp->send(this, handshake_message.message());
	}catch(std::exception e){
		return false;
	}

	return _last_message == message::_handshake;
}

bool PeerWireProtocol::interested(){
	std::cout << "interested" << std::endl;

	try{
		message::interface& intereseted_message = message::interested::create();
		_tcp->send(this, intereseted_message.message());
	}catch(std::exception e){
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

			while(request()){
				boost::this_thread::sleep(boost::posix_time::milliseconds(20));
			}
		}
	}
}

void PeerWireProtocol::yeild(){
	if(!_conversation){
		_conversation = new boost::thread(boost::bind(&PeerWireProtocol::conversation, this));
	}
	_conversation->yield();
}