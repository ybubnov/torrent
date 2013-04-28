#pragma once

#include "BencodeElement.h"

namespace bencode{
	namespace type{
		typedef class BencodeInteger : public BencodeElement{
			private:
				int64_t integer;

			public:
				BencodeInteger(int64_t integer);
				void* decode();
				std::vector<char> bencode();
				static int64_t decode(BencodeElement* decoded);
		}integer;
	}
}