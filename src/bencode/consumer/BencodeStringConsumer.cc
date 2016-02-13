#include "bencode/consumer/string.h"

#if 0

using namespace bencode::consumer;

BencodeStringConsumer::BencodeStringConsumer(){
}

BencodeStringConsumer::~BencodeStringConsumer(){
    destination.clear();
}

void BencodeStringConsumer::set(bencode::element* encoded){
    std::vector<char> vdata = encoded->bencode();
    destination.insert(destination.end(), vdata.begin(), vdata.end());
}

void BencodeStringConsumer::dispose(){
    destination.clear();
}

std::vector<char> BencodeStringConsumer::vector(){
    return std::vector<char>(destination.begin(), destination.end());
}

#endif
