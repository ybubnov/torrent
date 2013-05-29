#pragma once

#include <string>
#include <sstream>
#include <list>
#include <iomanip>
#include <cstring>

/*provides blocks for the secure hash algorithm
 */

namespace encryption{
	namespace sha{
		namespace provider{
			typedef class BlockProviderInterface{
				public:
                    virtual std::list<long> block() = 0;                            //returns sixteen 4-bytes words
			}interface;
		}
	}
}
