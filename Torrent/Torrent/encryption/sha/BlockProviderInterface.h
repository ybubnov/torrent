#pragma once

#include <string>
#include <sstream>
#include <list>
#include <iomanip>

namespace encryption{
	namespace sha{
		namespace provider{
			typedef class BlockProviderInterface{
				public:
					virtual std::list<long> block() = 0;
			}interface;
		}
	}
}