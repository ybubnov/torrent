#include "incoming_runner.h"

using namespace network::bittorrent::peer_wire;
using namespace network::bittorrent;

void incoming_runner::initialize(){
    _this = 0;
}

incoming_runner::incoming_runner(
    network::game_overable* dad,
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
    std::vector<char> info_hash,
    std::vector<char> peer_id){

    _incoming = new incoming_protocol(stranger, piece_control, file, info_hash, peer_id);
    _dad = dad;

    initialize();
}

incoming_runner::incoming_runner(
    network::game_overable* dad,
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
    std::string info_hash,
    std::vector<char> peer_id){

    _incoming = new incoming_protocol(stranger, piece_control, file, info_hash, peer_id);
    _dad = dad;

    initialize();
}

incoming_runner::incoming_runner(
    network::game_overable *dad,
    peer_wire::peer stranger,
    network::bittorrent::piece::control* piece_control,
    network::bittorrent::io::basic_file* file,
    std::string info_hash,
    std::string peer_id){

    _incoming = new incoming_protocol(stranger, piece_control, file, info_hash, peer_id);
    _dad = dad;

    initialize();
}

incoming_runner::~incoming_runner(){
    delete _this;
}


void incoming_runner::invoke(){
    for(unsigned int times = 0; times < 2; times++){
        if(!_incoming->connect()){
            return;
        }

        try{
            if(_incoming->handshake()){
                if(_incoming->interested()){
                    while(_incoming->request());
                }
            }
        }catch(std::exception e){
            _incoming->disconnect();
        }
    }

    _dad->game_over();
}


void incoming_runner::interrupt(){
    _this->timed_join(boost::posix_time::milliseconds(0));
}

void incoming_runner::timed_join(long millisecond){
    _this->timed_join(boost::posix_time::milliseconds(millisecond));
}

void incoming_runner::yeild(){
    if(!_this){
        _this = new boost::thread(boost::bind(&incoming_runner::invoke, this));
    }

    _this->yield();
}

