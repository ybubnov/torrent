#include "bencoder/consumer/stream.h"
#if 0

using namespace bencode::consumer;

BencodeStreamConsumer::BencodeStreamConsumer(std::wstring path){
    _path = path;

    destination.open(path);

    if(!destination.is_open()){
        throw std::bad_exception();
    }

    destination.close();
}

BencodeStreamConsumer::~BencodeStreamConsumer(){
    if(destination.is_open()){
        destination.close();
    }
}

void BencodeStreamConsumer::set(bencode::element* encoded){
    destination.open(_path);

    if(!destination.is_open()){
        throw std::bad_exception();
    }

    std::vector<char> vdata = encoded->bencode();
    char* buf = new char[vdata.size() + 1];

    std::copy(vdata.begin(), vdata.end(), buf);

    destination.write(buf, vdata.size());
    destination.close();
}

void BencodeStreamConsumer::dispose(){
    if(destination.is_open()){
        destination.close();
    }
}

std::vector<char> BencodeStreamConsumer::vector(){
    std::vector<char> vdata;
    char sbuf[65535];

    boost::filesystem::ifstream source;

    source.open(_path);

    if(!source.is_open()){
        throw std::bad_exception();
    }


    while(!source.eof()){
        source.read(sbuf, sizeof(sbuf));

        if(source.gcount() < sizeof(sbuf)){
            vdata.insert(vdata.end(), sbuf, sbuf + source.gcount());
            break;
        }else{
            vdata.insert(vdata.end(), sbuf, sbuf + sizeof(sbuf));
        }
    }

    source.close();
    return vdata;
}

#endif
