#include "Utf8Encoder.h"

std::string Utf8Encoder::encode(std::wstring source){
	std::string destination;
	wchar_t symbol;
	unsigned char code0;
	unsigned char code1;
	unsigned char code2;

	for(std::wstring::iterator it = source.begin(); it != source.end(); it++){
		symbol = static_cast<wchar_t>(*it);

		code0 = 0;
		code1 = 0;
		code2 = 0;

		if(symbol < 0x00000080){
			code0 = static_cast<char>(*it);
			destination.push_back(code0);
		}else if(symbol < 0x00000800){
			code0 = symbol % 0x40;
			code1 = (symbol - code0) / 0x40;

			destination.push_back(0xc0 | code1);
			destination.push_back(0x80 | code0);
		}else{
			code0 = symbol % 0x40;
			code1 = ((symbol - code0) / 0x40) % 0x40;
			code2 = (symbol - code0 - (0x40 * code1)) / 0x1000;

			destination.push_back(0xe0 | code2);
			destination.push_back(0x80 | code1);
			destination.push_back(0x80 | code0);
		}
	}

	return destination;
}