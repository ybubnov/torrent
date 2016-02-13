#include "bencode/parser.h"

using namespace bencode;

const char BencodeParser::_delimeter = ':';
const std::vector<char> BencodeParser::_integer = init("i");
const std::vector<char> BencodeParser::_string = init("0123456789");
const std::vector<char> BencodeParser::_list = init("l");
const std::vector<char> BencodeParser::_dictionary = init("d");
const std::vector<char> BencodeParser::_end = init("e");

BencodeParser::BencodeParser(bencode::provider::interface& source) : source(source){
}

std::vector<char> BencodeParser::init(const char* values){
    std::vector<char> value(values, values + std::strlen(values));
    return value;
}

BencodeElement* BencodeParser::node(){
    std::vector<char> type = source.peek();

    if(std::equal(type.begin(), type.end(), _integer.begin())){
        return integer();
    }else if(std::equal(type.begin(), type.end(), _list.begin())){
        return list();
    }else if(std::equal(type.begin(), type.end(), _dictionary.begin())){
        return dictionary();
    }else if(std::search(_string.begin(), _string.end(), type.begin(), type.end()) != _string.end()){
        return string();
    }

    throw std::bad_exception();
}


BencodeElement* BencodeParser::string(){
    std::vector<char> next = source.get();
    std::vector<char> nextstr;
    std::vector<char> streamlen;
    std::vector<char> streamstr;

    while(next[next.size() - 1] != _delimeter){
        streamlen = next;
        nextstr = source.get();
        next.insert(next.end(), nextstr.begin(), nextstr.end());
    }

    char* streambuf = new char[streamlen.size() + 1];
    memset(streambuf, 0, streamlen.size() + 1);
    std::copy(streamlen.begin(), streamlen.end(), streambuf);

    int length = std::atoi(streambuf);
    streamstr = source.get(length);

    delete[] streambuf;

    return new type::string(streamstr);
}

BencodeElement* BencodeParser::integer(){
    source.get();
    std::vector<char> next = source.get();
    std::string streamstr;
    std::stringstream stream;

    while(!std::equal(next.begin(), next.end(), _end.begin())){
        streamstr.insert(streamstr.end(), next.begin(), next.end());
        next = source.get();
    }

    int64_t streamval;

    stream << streamstr;
    stream >> streamval;

    return new type::integer(streamval);
}

BencodeElement* BencodeParser::list(){
    bencode::list streamlist;
    std::vector<char> next;
    source.get();

    next = source.peek();
    while(!std::equal(next.begin(), next.end(), _end.begin())){
        streamlist.push_back(node());
        next = source.peek();
    }

    source.get();
    return new type::list(streamlist);
}

BencodeElement* BencodeParser::dictionary(){
    bencode::map streamdictionary;
    std::vector<char> next;

    source.get();
    next = source.peek();
    while(!std::equal(next.begin(), next.end(), _end.begin())){
        BencodeElement* first = node();
        BencodeElement* second = node();
        streamdictionary.insert(bencode::pair(first, second));    
        next = source.peek();
    }
    source.get();
    return new type::dictionary(streamdictionary);
}
