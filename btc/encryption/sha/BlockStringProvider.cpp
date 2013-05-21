#include "BlockStringProvider.h"

using namespace encryption::sha::provider;

BlockStringProvider::BlockStringProvider(std::string source){
	length = source.size();
	source.push_back((char)0x80);

	std::stringstream stream;
	stream << source;

	char value[5];
	long qbyte;

	while(!stream.eof()){
		memset(value, 0, sizeof(value));
		stream.read(value, 4);

		qbyte = 0;
		qbyte |= ((static_cast<long>(value[0]) & 0xff) << 0x18);
		qbyte |= ((static_cast<long>(value[1]) & 0xff) << 0x10);
		qbyte |= ((static_cast<long>(value[2]) & 0xff) << 0x8);
		qbyte |= ((static_cast<long>(value[3]) & 0xff));
		bytelist.push_back(qbyte);
	}
}

std::list<long> BlockStringProvider::block(){
	int size = bytelist.size();

	if(length == -1 && bytelist.size()){
		std::list<long> byteblock = bytelist;
		bytelist.clear();
		return byteblock;
	}else if(length == -1 && !bytelist.size()){
		throw std::bad_exception();
	}
	

	if(size < 16){
		std::list<long> byteblock(16 - size);
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