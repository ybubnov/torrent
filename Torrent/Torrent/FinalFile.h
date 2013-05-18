#pragma once

#include <fstream>
#include <vector>
#include <boost/thread.hpp>

#include "global.h"

class FinalFile{
	private:
		boost::mutex _guard;

		std::vector<int> _writed;

		std::fstream _file;
		std::string _path;
		long _piece_size;
		int64_t _file_length;
		int64_t _downloaded;

	public:
		FinalFile(std::string path, int64_t file_length, long piece_size);
		int64_t left();
		void write(std::vector<char>& data, long piece_number);
};