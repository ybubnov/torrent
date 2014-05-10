#include "Interested.h"

using namespace network::bittorrent::message;

Interested::Interested(){
    data = std::vector<char>(3, 0);
    data.insert(data.end(), (char)0x1);
    data.insert(data.end(), (char)0x2);
}

Interested Interested::create(){
    return Interested();
}

std::vector<char> Interested::message(){
    return data;
}

type Interested::what(){
    return _interested;
}
