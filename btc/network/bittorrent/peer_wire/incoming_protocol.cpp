#include "incoming_protocol.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

const long incoming_protocol::block_length = 0x4000;

void incoming_protocol::initialize(){
    _offset = 0;
    _begin = 0;
    _tcp = 0;
    _current_piece = 0;

    _last_message = message::_undefined;

    //in case when piece length defined in torrent file larger than
    //piece avaliable for download via 'peer' protocol
    //set length of downloadable part of piece
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
            //rip information block from message
            _block = message::piece::block(incoming);

            //save information block in buffer
            _buffer.insert(_buffer.end(), _block.begin() + _offset, _block.end());
        }catch(std::exception e){
            std::cout << "INVALID BLOCK RECIEVED, block size:" << _block.size() << " offset:" << _offset << std::endl;

            _last_message = message::_undefined;
            return;
        }

    }else{
        std::cout << "GET UNDEFINED MESSAGE" << std::endl;
        _last_message = message::_undefined;
    }
}

void incoming_protocol::pull_piece(){
    //write to file if buffer is full
    _file->write(_buffer, _current_piece->index());

    //reset all variables
    _buffer = std::vector<char>();
    _begin = 0;
    _offset = 0;

    //mark piece as downloaded
    _piece_control->downloaded(_current_piece);

    //get next piece to download
    next_piece();
}

void incoming_protocol::next_piece(){
    //get new instance of piece
    _current_piece = _piece_control->next_piece();

    int64_t last_piece_length = _file->length() -
            (_piece_control->count() - 1) * _piece_control->piece_length();

    //if that piece is the last
    if((_piece_control->count() - 1) == _current_piece->index()){
        std::cout << "LAST INDEX:" << _current_piece->index()
                  << " last_piece_length:" << last_piece_length << std::endl;

        //and longer than '_piece_length'
        if(_piece_length > last_piece_length){
            std::cout << "LENGTH HAS CHANGED" << std::endl;
            _piece_length = last_piece_length;
        }

    }
}

bool incoming_protocol::last_piece(){
    int64_t last_piece_length = _file->length() -
            (_piece_control->count() - 1) * _piece_control->piece_length();

    //if downloadable piece is the last
    if((_piece_control->count() - 1) == _current_piece->index()){
        if(_begin + _piece_length < last_piece_length){
            //next part of the last peice
            _begin += _piece_length;
            _offset = 0;

            //edge part of the last piece
            if(_begin + _piece_length > last_piece_length){
                _offset = _begin;
                _begin = last_piece_length - _piece_length;
                _offset -=  _begin;
            }

        }else{
            std::cout << "LAST PIECE HAS WRITTEN" << std::endl;
            //write loaded piece to file
            pull_piece();
        }

        return true;
    }

    return false;
}

void incoming_protocol::part_piece(){
    if(last_piece()){
        return;
    }

    if(_begin + _piece_length < _piece_control->piece_length()){
        //download next part of piece
        _begin += _piece_length;

        //offset in piece buffer
        _offset = 0;

        //if next part is out of piece length
        //then load edge part
        if(_begin + _piece_length > _piece_control->piece_length()){
            _offset = _begin;
            _begin = _piece_control->piece_length() - _piece_length;
            _offset -= _begin;
        }

    }else{
        //write buffer to file
        pull_piece();
    }
}


bool incoming_protocol::refresh(){
    long previous_index = _current_piece->index();

    _buffer = std::vector<char>();
    _begin = 0;
    _offset = 0;

    try{
        //get new 'piece' instance to download
        next_piece();

        //twice request of the last part is unnecessary
        if(previous_index == _current_piece->index() && _piece_control->left() == 1){
            return false;
        }
    }catch(std::exception e){
        return false;
    }

    return true;
}

bool incoming_protocol::request(){

    if(!_current_piece){
        next_piece();
    }

    try{
        //send 'request' message to 'peer'
        message::request request_message = message::request::create(_current_piece->index(), _begin, _piece_length);
        _last_message = message::_undefined;
        _tcp->send(this, request_message.message(), _piece_length + 0xd);

        //imaginary 'request_handle' call
        //if response on 'piece' message is not really 'piece'
        if(_last_message != message::_piece){
            std::cout << "BAD REQUEST: NOT A PIECE" << std::endl;
            throw std::exception();
        }

        part_piece();

    }catch(boost::system::system_error e){
        //mean network problmes; 'tcp::socket' usually raise 'boost::system::system_error'
        //user should try to reconnect to 'peer'
        std::cout << "FAILURE index: " << _current_piece->index() << " begin: "
                  << _begin << " length: " << _piece_length
                  << " loaded:" << _file->downloaded() << " buffer: " << _buffer.size() << std::endl;

        throw e;
    }catch(std::exception e){
        std::cout << "UNEXPECTED EXCEPTION" << std::endl;

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
    }catch(std::exception){
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
    }catch(boost::system::system_error){
        std::cout << "DISCONNECTION PROBLEMS" << std::endl;
        return false;
    }

    return true;
}

