#include "SecureHashAlgorithm.h"
#include <iostream>

using namespace encryption::sha;

SecureHashAlgorithm::SecureHashAlgorithm(provider::interface* source){
	this->source = source;
}

long SecureHashAlgorithm::leftrotate(long value, long n){
	long bit;

	for(int i = 0; i < n; i++){
		bit = value & 0x80000000 ? 1 : 0;
		value <<= 1;
		value |= bit;
	}

	return value;
}

std::string SecureHashAlgorithm::url_hash(){
	if(hashes.size() == 0){
		throw std::bad_exception();
	}

	std::string result;
	char code0;
	char code1;
	char code2;
	char code3;

	for(std::list<long>::iterator it = hashes.begin(); it != hashes.end(); it++){
		code0 = (char)(((*it) & 0xff000000) >> 0x18);
		code1 = (char)(((*it) & 0x00ff0000) >> 0x10);
		code2 = (char)(((*it) & 0x0000ff00) >> 0x8);
		code3 = (char)(((*it) & 0x000000ff));

		result.push_back(code0);
		result.push_back(code1);
		result.push_back(code2);
		result.push_back(code3);
	}

	return result;
}

std::string SecureHashAlgorithm::hash(){
	long h0 = 0x67452301;
	long h1 = 0xefcdab89;
	long h2 = 0x98badcfe;
	long h3 = 0x10325476;
	long h4 = 0xc3d2e1f0;


	long a;
	long b;
	long c;
	long d;
	long e;

	long f;
	long k;
	long w[80];

	std::string hash;

	try{
		while(true){
			std::list<long> bytelist = source->block();
			std::list<long>::iterator it = bytelist.begin();

			for(int i = 0; i < 16; i++, it++){
				w[i] = *it;
			}

			for(int i = 16; i < 80; i++){
				long shift = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
				w[i] = leftrotate(shift, 1);
			}

			a = h0;
			b = h1;
			c = h2;
			d = h3;
			e = h4;

			for(int i = 0; i < 80; i++){
				if(i < 20){
					f = (b & c) | ((~b) & d);
					k = 0x5a827999;
				}else if(i < 40){
					f = b ^ c ^ d;
					k = 0x6ed9eba1;
				}else if(i < 60){
					f = (b & c) | (b & d) | (c & d);
					k = 0x8f1bbcdc;
				}else if(i < 80){
					f = b ^ c ^ d;
					k = 0xca62c1d6;
				}

				long temp = leftrotate(a, 5) + f + e + k + w[i];
				e = d;
				d = c;
				c = leftrotate(b, 30);
				b = a;
				a = temp;
			}

			h0 = h0 + a;
			h1 = h1 + b;
			h2 = h2 + c;
			h3 = h3 + d;
			h4 = h4 + e;
		}
	}catch(std::bad_exception e){
		std::string what = e.what();
		std::stringstream stream;

		hashes.push_back(h0);
		hashes.push_back(h1);
		hashes.push_back(h2);
		hashes.push_back(h3);
		hashes.push_back(h4);

		stream << std::hex << std::setw(8) << std::setfill('0') << h0;
		stream << std::hex << std::setw(8) << std::setfill('0') << h1;
		stream << std::hex << std::setw(8) << std::setfill('0') << h2;
		stream << std::hex << std::setw(8) << std::setfill('0') << h3;
		stream << std::hex << std::setw(8) << std::setfill('0') << h4;

		hash = stream.str();
	}

	return hash;
}

