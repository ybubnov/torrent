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

	class comparator{
		public:
			bool operator()(bencode::element* first, bencode::element* second) const {
				if(first->compare(second) < 0){
					return true;
				}

				return false;
			}
	};

	typedef std::list<bencode::element*> list;
	typedef std::map<bencode::element*, bencode::element*, comparator> map;
	typedef std::pair<bencode::element*, bencode::element*> pair;
}