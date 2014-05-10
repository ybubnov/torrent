#include "BlockVectorProvider.h"

using namespace encryption::sha::provider;

BlockVectorProvider::BlockVectorProvider(std::vector<char> source){
    length = source.size();

    std::vector<unsigned char> incoming(source.begin(), source.end());
    incoming.push_back(0x80);

    long qbyte = 0;
    long mask = 0x18;

    for(unsigned int i = 0; i < incoming.size(); i++){
        qbyte |= ((static_cast<long>(incoming[i]) & 0xff) << mask);

        if(mask == 0){
            bytelist.push_back(qbyte);
            qbyte = 0;
            mask = 0x20;
        }


        mask -= 0x8;
    }

    bytelist.push_back(qbyte);
}

std::list<long> BlockVectorProvider::block(){
    int size = bytelist.size();

    if(length == -1 && bytelist.size()){
        std::list<long> byteblock = bytelist;
        bytelist.clear();
        return byteblock;
    }else if(length == -1 && !bytelist.size()){
        throw std::bad_exception();
    }
    

    if(size < 16){
        std::list<long> byteblock(16 - size, 0);
        std::list<long>::iterator it;

        byteblock.insert(byteblock.begin(), bytelist.begin(), bytelist.end());
        bytelist.clear();

        it = byteblock.begin();
        for(int i = 0; i < size - 1; i++, it++);


        if(16 - size >= 2){
            byteblock.pop_back();
            byteblock.pop_back();
        }else{
            bytelist = std::list<long>(14);    
            bytelist.push_back((long)((length * 8) / pow(2.0, 32.0)));
            bytelist.push_back(length * 8);

            length = -1;
            return byteblock;
        }

        byteblock.push_back((long)((length * 8) / pow(2.0, 32.0)));
        byteblock.push_back(length * 8);

        length = -1;
        return byteblock;
    }else{
        std::list<long> byteblock;
        std::list<long>::iterator it = bytelist.begin();

        for(int i = 0; i < 16; i++, it++);
        byteblock.insert(byteblock.begin(), bytelist.begin(), it);
        bytelist.erase(bytelist.begin(), it);

        return byteblock;
    }
}
