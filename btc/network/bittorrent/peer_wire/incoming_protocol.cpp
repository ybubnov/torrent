#include "incoming_protocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

const long incoming_protocol::block_length = 0x4000;

void incoming_protocol::initialize(){
    _offset = 0;
    _begin = 0;
    _tcp = 0;
    _conversation = 0;
    _last_message = message::_undefined;

    if(_piece_control->piece_length() < incoming_protocol::block_length){
        _piece_length = _piece_control->piece_length();
    }else{
        _piece_length = incoming_protocol::block_length;
    }
}

incoming_protocol::incoming_protocol(
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
    std::vector<char> info_hash,
    std::vector<char> peer_id) : _peer(stranger){

    _piece_control = piece_control;
    _info_hash = info_hash;
    _peer_id = peer_id;
    _file = file;

    initialize();
}

incoming_protocol::incoming_protocol(
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
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

incoming_protocol::incoming_protocol(
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
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
    _file = file;

    initialize();
}

incoming_protocol::~incoming_protocol(){
    //_tcp->stop();
    delete _tcp;
}

void incoming_protocol::response_handle(std::istream& response){
    char buf[0x7fff];
    memset(buf, 0, sizeof(buf));
    response.read(buf, sizeof(buf));

    std::vector<char> incoming(buf, buf + sizeof(buf));
    _incoming = incoming;

    if(message::handshake::is(incoming)){
        _last_message = message::_handshake;
        /*
        try{
            incoming = message::handshake::crop(incoming);

            if(message::bitfield::is(incoming)){
                _have = message::bitfield::field(incoming);
            }
        }catch(std::exception e){

        }*/

    }else if(message::unchoke::is(incoming)){
        _last_message = message::_unchoke;
    }else if(message::piece::is(incoming)){
        _last_message = message::_piece;
        std::vector<char> _block;

        try{
            _block = message::piece::block(incoming);
        }catch(std::exception e){
            std::cout << "BLOCK PROBLEM" << std::endl;

            _last_message = message::_undefined;
            return;
        }

        _buffer.insert(_buffer.end(), _block.begin() + _offset, _block.end());
    }else{
        _last_message = message::_undefined;
    }
}


void incoming_protocol::shift(){
    if(_begin + _piece_length < _piece_control->piece_length()){
        _begin += _piece_length;
        _offset = 0;

        if(_begin + _piece_length > _piece_control->piece_length()){
            _offset = _piece_length - (_piece_control->piece_length() - _begin);
            _begin = _piece_control->piece_length() - _piece_length;

        }
    }else{
        _file->write(_buffer, _current_piece->index());

        //std::cout << "has written " << _current_piece->index() << std::endl;

        _buffer = std::vector<char>();
        _piece_control->downloaded(_current_piece);
        _begin = 0;
        _offset = 0;

        _current_piece = _piece_control->next_piece();

        if(_piece_control->piece_length() < incoming_protocol::block_length){
            _piece_length = _piece_control->piece_length();
        }else{
            _piece_length = incoming_protocol::block_length;
        }
    }
}


bool incoming_protocol::refresh(){

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

    if(_piece_control->piece_length() < incoming_protocol::block_length){
        _piece_length = _piece_control->piece_length();
    }else{
        _piece_length = incoming_protocol::block_length;
    }

    return true;
}

bool incoming_protocol::request(){
    long piece_begin;
    long piece_index;
    long piece_length;
    int64_t left_length;

    _current_piece = _piece_control->next_piece();

    try{
        piece_begin = _begin;
        piece_index = _current_piece->index();

        left_length = _file->left() - (int64_t)_buffer.size();

        if(left_length <= 0){
            throw std::bad_exception();
        }else if(left_length < (int64_t)_piece_length){
            std::cout << "left :" << left_length << std::endl;

            piece_length = (long)left_length;
        }else{
            piece_length = _piece_length;
        }


        message::request request_message = message::request::create(piece_index, piece_begin, piece_length);


        _last_message = message::_undefined;
        _tcp->send(this, request_message.message(), piece_length + 0xd);

        //imaginary 'request_handle' call
        if(_last_message != message::_piece){
            throw std::exception();
        }


        shift();
    }catch(boost::system::system_error e){
        std::cout << "FAILURE" << std::endl;

        throw e;

        /*std::cout << "begin = " << piece_begin << " index = " << piece_index << " length = " << piece_length << std::endl;
        std::cout << "FAILURE" << std::endl;

        _tcp->stop();
        delete _tcp;
        _tcp = 0;


        connect();

        return false;*/
    }catch(std::exception e){
        std::cout << "BAD REQUEST" << std::endl;

        if(_piece_control->left()){
            return refresh();
        }

        return false;
    }

    return true;
}

bool incoming_protocol::handshake(){
    std::cout << "handshake" << std::endl;

    try{
        message::handshake handshake_message = message::handshake::create(_info_hash, _peer_id);
        _tcp->send(this, handshake_message.message());
    }catch(std::exception e){
        return false;
    }

    return _last_message == message::_handshake;
}

bool incoming_protocol::interested(){
    std::cout << "interested" << std::endl;

    try{
        message::interested intereseted_message = message::interested::create();
        _tcp->send(this, intereseted_message.message());
    }catch(std::exception e){
        return false;
    }

    return _last_message == message::_unchoke;
}


bool incoming_protocol::connect(){
    try{
        if(!_tcp){
            _tcp = new network::tcp::protocol(_peer.ip(), _peer.port());
        }
    }catch(std::exception e){
        //_dad->game_over();
        std::cout << "BAD CONNECTION DETECTED" << std::endl;
        return false;
    }

    return true;
}

bool incoming_protocol::disconnect(){
    try{
        _tcp->stop();
        delete _tcp;
        _tcp = 0;
    }catch(boost::system::system_error e){
        std::cout << "DISCONNECTION PROBLEMS" << std::endl;
        return false;
    }

    return true;
}

