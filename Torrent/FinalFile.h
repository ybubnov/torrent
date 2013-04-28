#pragma once

#include <fstream>
#include <vector>
#include <boost/thread.hpp>

class FinalFile{
	private:
		boost::mutex _guard;

		std::fstream _file;
		std::string _path;
		long _piece_size;
	public:
		FinalFile(std::string path, long piece_size);

		void write(std::vector<char> data, long piece_number);
};