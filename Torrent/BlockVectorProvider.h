#pragma once

#include <vector>
#include "BlockProviderInterface.h"

class BlockVectorProvider : public BlockProviderInterface{
	private:
		int length;
		std::list<long> bytelist;

	public:
		BlockVectorProvider(std::vector<char> source);
		std::list<long> block();
};