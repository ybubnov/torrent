#include "bencode/type/list.h"

using namespace bencode::type;

BencodeList::BencodeList(bencode::list list){
    this->list.insert(this->list.end(), list.begin(), list.end());
}

BencodeList::~BencodeList(){
    for(bencode::list::iterator element_ptr = list.begin();
        element_ptr != list.end(); element_ptr++){
        delete *element_ptr;
    }
}

void* BencodeList::decode(){
    return static_cast<void*>(&list);
}

std::vector<char> BencodeList::bencode(){
    std::vector<char> streamstr;
    std::vector<char> nextstr;

    streamstr.push_back('l');
    for(bencode::list::iterator it = list.begin(); it != list.end(); it++){
        nextstr = (*it)->bencode();
        streamstr.insert(streamstr.end(), nextstr.begin(), nextstr.end());
    }
    streamstr.push_back('e');

    return streamstr;
}

bencode::list BencodeList::decode(BencodeElement* decoded){
    return *static_cast<bencode::list*>(decoded->decode());
}
