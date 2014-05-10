#include "peer.h"

using namespace network::bittorrent::peer_wire;

peer::peer(std::string ip, std::string port){
    _ip = ip;
    _port = port;
}

std::string peer::ip(){
    return _ip;
}

std::string peer::port(){
    return _port;
}
