#pragma once

#include "BencodeElement.h"

namespace bencode{
	class comparator{
		public:
			bool operator()(BencodeElement* first, BencodeElement* second) const {
				if(first->compare(second) < 0){
					return true;
				}

				return false;
			}
	};
}