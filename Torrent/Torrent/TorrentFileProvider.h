#pragma once

#include <algorithm>

#include "bencode.h"
#include "encryption.h"

class DownloadFile{
	public:
		int64_t length;
		std::string path;
		std::string md5sum;

		DownloadFile(int64_t length, std::string path, std::string md5sum = ""){
			this->length = length;
			this->path = path;
			this->md5sum = md5sum;
		}
};


class TorrentFileProvider{

	public:
		TorrentFileProvider(bencode::element* dictionary);

		/*std::string creation_date();
		std::string comment();
		std::string created_by();
		std::string encoding();
		long piece_length();
		std::list<std::string> pieces();*/

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
};