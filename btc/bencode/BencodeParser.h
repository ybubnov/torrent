#pragma once

#include <list>
#include <string>
#include <fstream>
#include <map>
#include <hash_map>
#include <algorithm>
#include <functional>
#include <typeinfo>

#include "type.h"
#include "provider.h"

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
	}parser;
}