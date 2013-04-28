#pragma once

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>

typedef long long int64_t;

namespace bencode{
	typedef class BencodeElement{
		private:
			static int64_t elements;
			int64_t hash;

		public:
			BencodeElement();
			int compare(BencodeElement* belement);
			int get_hash();

			static void gc();

			virtual void* decode() = 0;												//convert to real type
			virtual std::vector<char> bencode() = 0;								//convert to bencode
	}element;
}