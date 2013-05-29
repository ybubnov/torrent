#include "BencodeString.h"

using namespace bencode::type;

BencodeString::BencodeString(std::vector<char> string){
	this->string = string;
}

void* BencodeString::decode(){
	return static_cast<void*>(&string);
}

std::vector<char> BencodeString::bencode(){
	std::stringstream strlength;
	std::string length;
	std::vector<char> streamstr;
	
	strlength << string.size();
	length = strlength.str();
	streamstr.insert(streamstr.begin(), length.begin(), length.end());
	
	streamstr.push_back(':');
	streamstr.insert(streamstr.end(), string.begin(), string.end());

	return streamstr;
}

std::vector<char> BencodeString::decode(BencodeElement* decoded){
	return *static_cast<std::vector<char>*>(decoded->decode());
}
