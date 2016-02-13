#include "bencode/provider/stream.h"

using namespace bencode::provider;

BencodeStreamProvider::BencodeStreamProvider(std::string path){
    try{
        source.open(path.c_str(), std::fstream::binary);
    }catch(std::fstream::failure fail){
        source.close();
        throw std::bad_exception();
    }
}

BencodeStreamProvider::BencodeStreamProvider(std::wstring path){
    try{
        source.open(path.c_str(), std::fstream::binary);
    }catch(std::exception e){
        source.close();
        throw std::bad_exception();
    }
}

std::vector<char> BencodeStreamProvider::get(int length){
    char sbuffer[65535]; 
    std::vector<char> data;
    int read_size = sizeof(sbuffer);

    do{
        memset(sbuffer, 0, sizeof(sbuffer));

        if(length > read_size){
            source.read(sbuffer, read_size);
            data.insert(data.end(), sbuffer, sbuffer + read_size);
            length -= read_size;

        }else{
            source.read(sbuffer, length);
            data.insert(data.end(), sbuffer, sbuffer + length);
            break;
        }
    }while(length);

    return data;
}

std::vector<char> BencodeStreamProvider::get(){
    return get(1);
}

std::vector<char> BencodeStreamProvider::peek(){
    std::streamoff offset = source.tellg();
    std::vector<char> streamval = get(1);

    source.seekg(offset, std::fstream::beg);
    return streamval;
}

BencodeStreamProvider::~BencodeStreamProvider(){
    if(source.is_open()){
        source.close();
    }
}
