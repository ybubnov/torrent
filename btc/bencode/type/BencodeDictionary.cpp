#include "BencodeDictionary.h"

using namespace bencode::type;

BencodeDictionary::BencodeDictionary(bencode::map dictionary){
	this->dictionary.insert(dictionary.begin(), dictionary.end());
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