#include "BencodeInteger.h"

using namespace bencode::type;

BencodeInteger::BencodeInteger(int64_t integer){
	this->integer = integer;
}

void* BencodeInteger::decode(){
	return static_cast<void*>(&integer);
}

std::vector<char> BencodeInteger::bencode(){
	std::stringstream buffer;
	std::string streamlen;
	std::vector<char> streamstr;

	buffer << integer;
	streamlen = buffer.str();
	streamstr.insert(streamstr.end(), streamlen.begin(), streamlen.end());

	streamstr.insert(streamstr.begin(), 'i');
	streamstr.insert(streamstr.end(), 'e');

	return streamstr;
}

int64_t BencodeInteger::decode(BencodeElement* decoded){
	return *static_cast<int64_t*>(decoded->decode());
}
