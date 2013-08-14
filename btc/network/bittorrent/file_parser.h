#pragma once

#include <algorithm>

#include "../../bencode.h"
#include "../../encryption.h"

#include "basic_parser.h"

typedef long long int64_t;

namespace network{
	namespace bittorrent{
        /*allows you to get the standard fields of the torrent-file
         */
        class file_parser : public basic_parser{

			public:
                file_parser(bencode::element* dictionary);                    //use parser.node() as an initializer
                ~file_parser();

                /*
                 *std::wstring created_by();
                 *std::wstring encoding();
                 */

                std::wstring name();
                std::wstring comment();
                std::string creation_date();

                std::list<file_stat> files();
				std::string size();
				std::string info_hash();
				std::list<std::string> announce_list();
				std::vector<char> pieces();

                size_t piece_length();

			private:
				std::map<std::string, bencode::element*> map;
				bencode::map source;
				bencode::map info_dictionary;

				std::string hash;
				std::list<std::string> announceList;
                std::list<file_stat> fileList;
        };
	}
}
