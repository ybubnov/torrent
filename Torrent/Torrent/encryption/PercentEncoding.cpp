#include "PercentEncoding.h"

using namespace encryption;

std::string PercentEncoding::encode(std::string source){
	std::string stream;
	int percent = 0;

	for(std::string::iterator it = source.begin(); it != source.end(); it++){
		if(!percent){
			stream += "%";
			percent = 1;
		}else{
			percent >>= 1;
		}

		stream += *it;
	}

	return stream;
}

std::string PercentEncoding::encode(std::vector<char> source){
	std::strstream stream;
	unsigned int ascii;
	std::string encoding;

	for(std::vector<char>::iterator it = source.begin(); it != source.end(); it++){
		ascii = *it;
		stream << "%" << std::hex << ((ascii & 0xf0) >> 0x4)
			<< std::hex << (ascii & 0xf);
	}
	
	encoding = std::string(stream.str()).substr(0, source.size() * 3);

	return encoding;
}