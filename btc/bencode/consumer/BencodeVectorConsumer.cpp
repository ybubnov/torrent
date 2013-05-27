#include "BencodeVectorConsumer.h"

using namespace bencode::consumer;

BencodeVectorConsumer::BencodeVectorConsumer(){
}

void BencodeVectorConsumer::set(bencode::element* encoded){
	std::vector<char> vdata = encoded->bencode();
	destination.insert(destination.end(), vdata.begin(), vdata.end());
}

void BencodeVectorConsumer::dispose(){
    destination.clear();
}

std::vector<char> BencodeVectorConsumer::vector(){
	return destination;
}
