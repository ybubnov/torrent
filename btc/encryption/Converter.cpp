#include "Converter.h"

using namespace encryption::codecvt;

std::wstring wstring_convert::codecvt_utf8_utf16(std::vector<char> data){
	short next_symbol;
	std::wstring converted;

	

	for(unsigned long i = 0; i < data.size(); i++){
		next_symbol = 0;

		if(!(((data[i] & 0xff) ^ 0xfc) >> 1)){								//1111110x

		}else if(!(((data[i] & 0xff) ^ 0xf8) >> 2)){						//111110xx

		}else if(!(((data[i] & 0xff) ^ 0xf0) >> 3)){						//11110xxx

		}else if(!(((data[i] & 0xff) ^ 0xe0) >> 4)){						//1110xxxx

		}else if(!(((data[i] & 0xff) ^ 0xc0) >> 5)){						//110xxxxx
			if(i + 1 < data.size()){
				next_symbol |= (data[i + 1] & 0x3f);
			}
			next_symbol |= ((data[i++] & 0x1f) << 0x6);
			converted.push_back((wchar_t)next_symbol);
		}else{
			next_symbol |= data[i];
			converted.push_back((wchar_t)next_symbol);
		}
	}

	return converted;
}


std::string wstring_convert::codecvt_utf16_utf8(std::wstring source){
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
