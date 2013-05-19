#pragma once

#include <fstream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

typedef long long int64_t;

namespace network{
	namespace bittorrent{
		namespace io{
			typedef class FinalFile{
				private:
					boost::mutex _guard;

					std::vector<int> _writed;
					std::vector<int64_t> _loaded;

					std::fstream _file;
					std::wstring _path;
					long _piece_size;
					int64_t _file_length;
					int64_t _downloaded;

				public:
					FinalFile(std::wstring path, int64_t file_length, long piece_size);
					int64_t left();
					int64_t length();
					int64_t downloaded();

					void write(std::vector<char>& data, long piece_number);
					void write(std::vector<char>& data, int64_t absolute_position);
			}file;
		}
	}
}
