#include "BlockVectorProvider.h"

BlockVectorProvider::BlockVectorProvider(std::vector<char> source){
	length = source.size();
	source.push_back(0x80);

	long qbyte = 0;
	long mask = 0x38;

	for(unsigned int i = 0; i < source.size(); i++){
		qbyte |= (static_cast<long>(source[i]) & 0xff) << mask;

		if(mask == 0){
			bytelist.push_back(qbyte);
			qbyte = 0;
			mask = 0x40;
		}

		if(mask == 0x28){
			mask = 0x0;
		}else{
			mask -= 0x8;
		}
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
		throw std::bad_exception("there is no more blocks");
	}
	

	if(size < 16){
		std::list<long> byteblock(16 - size);
		std::list<long>::iterator it;

		byteblock.insert(byteblock.begin(), bytelist.begin(), bytelist.end());
		bytelist.clear();

		it = byteblock.begin();
		for(int i = 0; i < size - 1; i++, it++);

		/*int index = length % 4;

		if(index != 0){
			long value = *(it);
			value |= 0x80 << ((3 - index)  * 8);
			*it = value;
		}else{
			*(++it) = 0x80000000; 
		}*/

		if(16 - size >= 2){
			byteblock.pop_back();
			byteblock.pop_back();
		}else{
			bytelist = std::list<long>(14);	
		}

		byteblock.push_back((long)((length * 8) / std::pow(2.0, 32.0)));
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