#include "bencode_comparator.h"
#include <map>

namespace bencode{
	typedef std::map<bencode::element*, bencode::element*, comparator> map;
}