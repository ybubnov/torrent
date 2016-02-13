#include "bencoder/type/dictionary.h"

using namespace bencode::type;

BencodeDictionary::BencodeDictionary(bencode::map dictionary){
    this->dictionary.insert(dictionary.begin(), dictionary.end());
}

BencodeDictionary::~BencodeDictionary(){
    for(bencode::map::iterator element_ptr = dictionary.begin();
        element_ptr != dictionary.end(); element_ptr++){
        delete element_ptr->first;
        delete element_ptr->second;
    }
}

void* BencodeDictionary::decode(){
    return static_cast<void*>(&dictionary);
}

std::vector<char> BencodeDictionary::bencode(){
    std::vector<char> first;
    std::vector<char> second;
    std::vector<char> streamstr;

    streamstr.push_back('d');
    for(bencode::map::iterator it = dictionary.begin(); it != dictionary.end(); it++){
        first = it->first->bencode();
        second = it->second->bencode();
        streamstr.insert(streamstr.end(), first.begin(), first.end());
        streamstr.insert(streamstr.end(), second.begin(), second.end());
    }

    streamstr.push_back('e');

    return streamstr;
}

bencode::map BencodeDictionary::decode(BencodeElement* decoded){
    return *static_cast<bencode::map*>(decoded->decode());
}
