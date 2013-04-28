#pragma once

#include <list>
#include <string>
#include <fstream>
#include <map>
#include <hash_map>
#include <algorithm>
#include <functional>
#include <typeinfo>

#include "map.h"
#include "list.h"
#include "pair.h"
#include "BencodeProviderInterface.h"
#include "BencodeElement.h"
#include "BencodeString.h"
#include "BencodeInteger.h"
#include "BencodeList.h"
#include "BencodeDictionary.h"

namespace bencode{
	typedef class BencodeParser{
		private:
			const static char _delimeter;
			const static std::vector<char> _end;
			const static std::vector<char> _integer;
			const static std::vector<char> _string;
			const static std::vector<char> _list;
			const static std::vector<char> _dictionary;

			bencode::provider::interface& source;
			static std::vector<char> init(const char* array);

		public:
			BencodeParser(bencode::provider::interface& source);
			bencode::element* node();

			bencode::element* string();
			bencode::element* integer();
			bencode::element* list();
			bencode::element* dictionary();

			/*void string(BencodeElement* bstring);
			void integer(BencodeElement* binteger);
			void list(BencodeElement* blist);
			void dictionary(BencodeElement* bdictionary);*/
	}parser;
}