#include "BencodeStreamConsumer.h"

using namespace bencode::consumer;

BencodeStreamConsumer::BencodeStreamConsumer(std::string path){
	this->path = path;
	destination.open(path.c_str(), std::fstream::binary | std::fstream::trunc);
}

BencodeStreamConsumer::~BencodeStreamConsumer(){
	if(destination.is_open()){
		destination.close();
	}
}

void BencodeStreamConsumer::set(bencode::element* encoded){
	/*void* data = encoded->bencode();

	destination.write(static_cast<char*>(data), 
		std::strlen(static_cast<char*>(data)));*/
}

void BencodeStreamConsumer::dispose(){
	destination.close();
}

std::vector<char> BencodeStreamConsumer::vector(){
	/*if(!destination.is_open()){
		throw std::bad_exception("access denied");	
	}

	std::streampos position = destination.tellp();
	std::ifstream stream;
	std::string tostring;
	char sbuffer[65535];

	destination.close();
	stream.open(path, std::fstream::binary);
	if(!stream.is_open()){
		stream.close();
		throw std::bad_exception("unextected end");
	}

	while(!destination.eof()){
		memset(static_cast<void*>(sbuffer), 0, sizeof(sbuffer));
		stream.read(sbuffer, sizeof(sbuffer));
		tostring.append(std::string(sbuffer));
	}

	stream.close();
	destination.open(path, std::fstream::binary);
	if(!destination.is_open()){
		destination.close();
		throw std::bad_exception("unextected end");
	}

	destination.seekp(position);

	return const_cast<char*>(tostring.c_str());*/
	return std::vector<char>();
}