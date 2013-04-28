#include "BencodeVectorProvider.h"

using namespace bencode::provider;

BencodeVectorProvider::BencodeVectorProvider(std::vector<char> source){
	this->source = source;
	index = 0;
}

BencodeVectorProvider::~BencodeVectorProvider(){
}

std::vector<char> BencodeVectorProvider::get(int length){ 
	std::vector<char> data;

	data = std::vector<char>(source.begin() + index, source.begin() + index + length);
	index += length;

	return data;
}

std::vector<char> BencodeVectorProvider::get(){
	return get(1);
}

std::vector<char> BencodeVectorProvider::peek(){
	return std::vector<char>(1, source.at(index));
}