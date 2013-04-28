#pragma once

#include <fstream>
#include "BlockProviderInterface.h"

class BlockStreamProvider : public BlockProviderInterface{
	private:
		int length;
		std::list<long> bytelist;

	public:
		BlockStreamProvider(std::string path);
		std::list<long> block();
};