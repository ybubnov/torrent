#pragma once

#include <fstream>
#include <vector>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/interprocess/exceptions.hpp>

typedef long long int64_t;

/*writes the downloaded pieces of the file in
 */

namespace network{
	namespace bittorrent{
		namespace io{
			typedef class FinalFile{
                private:

                    std::vector<int> _writed;                                       //recorded pieces
                    std::vector<int64_t> _loaded;                                   //recorded in the predetermined position

                    boost::filesystem::fstream _file;
					std::wstring _path;
					long _piece_size;
					int64_t _file_length;
					int64_t _downloaded;

				public:
					FinalFile(std::wstring path, int64_t file_length, long piece_size);
					~FinalFile();
					int64_t left();
					int64_t length();
					int64_t downloaded();

					void write(std::vector<char>& data, long piece_number);
					void write(std::vector<char>& data, int64_t absolute_position);
			}file;
		}
	}
}
