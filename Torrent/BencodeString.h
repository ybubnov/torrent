#pragma once

#include "BencodeElement.h"

namespace bencode{
	namespace type{
		typedef class BencodeString : public BencodeElement{
			private:
				std::vector<char> string;

			public:
				BencodeString(std::vector<char> string);
				void* decode();
				std::vector<char> bencode();
				static std::vector<char> decode(BencodeElement* decoded);
		}string;
	}
}