#pragma once

#include <algorithm>

#include "../../bencode.h"
#include "../../encryption.h"

typedef long long int64_t;

namespace network{
	namespace bittorrent{
		typedef class DownloadFile{
			public:
				int64_t length;
				std::wstring path;
				std::string md5sum;

				DownloadFile(int64_t length, std::wstring path, std::string md5sum = ""){
					this->length = length;
					this->path = path;
					this->md5sum = md5sum;
				}
		}download_file;


        /*allows you to get the standard fields of the torrent-file
         */
		typedef class TorrentFileParser{

			public:
                TorrentFileParser(bencode::element* dictionary);                    //use parser.node() as an initializer
                ~TorrentFileParser();

                /*
                 *std::wstring created_by();
                 *std::wstring encoding();
                 */

                std::wstring name();
                std::wstring comment();
                std::string creation_date();

				std::list<DownloadFile> files();
				std::string size();
				std::string info_hash();
				std::list<std::string> announce_list();
				std::vector<char> pieces();

				size_t piece_length();
				size_t announce_size();

			private:
				std::map<std::string, bencode::element*> map;
				bencode::map source;
				bencode::map info_dictionary;

				std::string hash;
				std::list<std::string> announceList;
				std::list<DownloadFile> fileList;
		}file_parser;
	}
}
