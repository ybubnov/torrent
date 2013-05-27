#include "BencodeStringConsumer.h"

using namespace bencode::consumer;

BencodeStringConsumer::BencodeStringConsumer(){
}

BencodeStringConsumer::~BencodeStringConsumer(){
	destination.clear();
}

void BencodeStringConsumer::set(bencode::element* encoded){
	/*void* data = encoded->bencode();

	destination.write(static_cast<char*>(data), 
		std::strlen(static_cast<char*>(data)));*/
}

void BencodeStringConsumer::dispose(){

}

std::vector<char> BencodeStringConsumer::vector(){
	//return const_cast<char*>(destination.str().c_str());
	return std::vector<char>();
}