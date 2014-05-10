#include "Have.h"

using namespace network::bittorrent::message;

Have::Have(long index){
    data = std::vector<char>(3, 0);

    data.insert(data.end(), 0x5);
    data.insert(data.end(), 0x4);

    data.insert(data.end(), (char)((index & 0xff000000) >> 0x18));
    data.insert(data.end(), (char)((index & 0x00ff0000) >> 0x10));
    data.insert(data.end(), (char)((index & 0x0000ff00) >> 0x8));
    data.insert(data.end(), (char)((index & 0x000000ff)));

}

std::vector<long> Have::have(std::vector<char> incoming){
    std::vector<long> have_response;
    long piece_index;
    long first_index = 5;
    long last_index = 9;

    if(incoming.at(4) != 0x4){
        throw std::exception();
    }

    while((long)incoming.size() > last_index){
        std::stringstream flow;
        std::string data(incoming.begin() + first_index, incoming.begin() + last_index);

        flow.read((char*)&piece_index, sizeof(long));

        have_response.push_back(piece_index);

        first_index = last_index + 5;
        last_index = first_index + 4;
    }

    return have_response;
}


Have Have::create(long index){
    return Have(index);
}

std::vector<char> Have::message(){
    return data;
}

type Have::what(){
    return _have;
}
