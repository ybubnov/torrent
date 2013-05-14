#pragma once

#include "BencodeElement.h"

namespace bencode{
	namespace type{
		typedef class BencodeList : public BencodeElement{
			private:
				bencode::list list;

			public:
				BencodeList(bencode::list list);
				void* decode();
				std::vector<char> bencode();
				static bencode::list decode(BencodeElement* decoded);
		}list;
	}
}