#pragma once

#include "BlockProviderInterface.h"

class BlockStringProvider : public BlockProviderInterface{
	private:
		int length;
		std::list<long> bytelist;

	public:
		BlockStringProvider(std::string source);
		std::list<long> block();
};