#pragma once

#include "BlockProviderInterface.h"

class SecureHashAlgorithm{
	private:
		BlockProviderInterface* source;
		std::list<long> hashes;
		long leftrotate(long value, long n);

	public:
		SecureHashAlgorithm(BlockProviderInterface* source);
		std::string hash();
		std::string url_hash();
};