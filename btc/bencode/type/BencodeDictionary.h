#pragma once

#include "BencodeElement.h"

namespace bencode{
	namespace type{
		typedef class BencodeDictionary : public BencodeElement{
			private:
				bencode::map dictionary;

			public:
				BencodeDictionary(bencode::map dictionary);
				void* decode();
				std::vector<char> bencode();
				static bencode::map decode(BencodeElement* decoded);
		}dictionary;
	}
}
