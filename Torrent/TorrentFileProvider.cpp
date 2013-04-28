#include "TorrentFileProvider.h"
#include <fstream>

TorrentFileProvider::TorrentFileProvider(bencode::element* dictionary){
	source = bencode::type::dictionary::decode(dictionary);
	std::string key;
	std::vector<char> vkey;
	bencode::element* value;


	for(bencode::map::iterator it = source.begin(); it != source.end(); it++){
		vkey = bencode::type::string::decode(it->first);
		key = std::string(vkey.begin(), vkey.end());
		value = it->second;

		map.insert(std::pair<std::string, bencode::element*>(key, value));
	}

	for(std::map<std::string, bencode::element*>::iterator it = map.begin(); it != map.end(); it++){
		if(!it->first.compare("info")){
			info_dictionary = bencode::type::dictionary::decode(it->second);
			break;
		}
	}
}

std::list<DownloadFile> TorrentFileProvider::files(){
	
	std::string key;
	std::vector<char> value;

	bencode::map bencode_map;
	bencode::map file_map;
	bencode::list path_list;
	bencode::list files_list;

	std::string path;
	long length;
	std::string md5sum;

	if(fileList.size()){
		return fileList;
	}

	for(std::map<std::string, bencode::element*>::iterator it = map.begin(); it != map.end(); it++){
		if(!it->first.compare("info")){
			bencode_map = bencode::type::dictionary::decode(it->second);
			break;
		}
	}

	for(bencode::map::iterator it = bencode_map.begin(); it != bencode_map.end(); it++){
		value = bencode::type::string::decode(it->first);
		key = std::string(value.begin(), value.end());

		if(!key.compare("files")){
			files_list = bencode::type::list::decode(it->second);

			for(bencode::list::iterator it = files_list.begin(); it != files_list.end(); it++){
				file_map = bencode::type::dictionary::decode(*it);		
				
				path = "";
				md5sum = "";
				length = 0;

				for(bencode::map::iterator it = file_map.begin(); it != file_map.end(); it++){
					value = bencode::type::string::decode(it->first);
					key = std::string(value.begin(), value.end());

					if(!key.compare("path")){
						path_list = bencode::type::list::decode(it->second);

						for(bencode::list::iterator it = path_list.begin(); it != path_list.end(); it++){
							value = bencode::type::string::decode(*it);
							path += "\\";
							path += std::string(value.begin(), value.end());
						}

					}else if(!key.compare("length")){
						length = (long)bencode::type::integer::decode(it->second);
					}else if(!key.compare("md5sum")){
						value = bencode::type::string::decode(it->second);
						path = std::string(value.begin(), value.end());
					}
				}

				fileList.push_back(DownloadFile(length, path, md5sum));
			}

			break;
		}
	}

	if(!fileList.size()){
		for(bencode::map::iterator it = bencode_map.begin(); it != bencode_map.end(); it++){
			value = bencode::type::string::decode(it->first);
			key = std::string(value.begin(), value.end());

			if(!key.compare("name")){
				value = bencode::type::string::decode(it->second);
				path = std::string(value.begin(), value.end());
			}else if(!key.compare("length")){
				length = (long)bencode::type::integer::decode(it->second);
			}else if(!key.compare("md5sum")){
				value = bencode::type::string::decode(it->second);
				md5sum = std::string(value.begin(), value.end());
			}
		}

		fileList.push_back(DownloadFile(length,path, md5sum));
	}


	return fileList;
}

size_t TorrentFileProvider::announce_size(){
	if(!announceList.size()){
		announceList = announce_list();
	}

	return announceList.size();
}

std::list<std::string> TorrentFileProvider::announce_list(){
	std::string value;
	bencode::list bencode_list;
	std::vector<char> decode;
	std::list<std::string> list;

	if(announceList.size()){
		return announceList;
	}

	for(std::map<std::string, bencode::element*>::iterator it = map.begin(); it != map.end(); it++){
		if(!it->first.compare("announce-list")){
			bencode_list = bencode::type::list::decode(it->second);
			break;
		}
	}

	for(bencode::list::iterator it = bencode_list.begin(); it != bencode_list.end(); it++){
		bencode::list decode_list = bencode::type::list::decode(*it);

		for(bencode::list::iterator it = decode_list.begin(); it != decode_list.end(); it++){
			decode = bencode::type::string::decode(*it);

			value = std::string(decode.begin(), decode.end());
			list.push_back(value);
		}
	}

	if(!bencode_list.size()){
		for(std::map<std::string, bencode::element*>::iterator it = map.begin(); it != map.end(); it++){
			if(!it->first.compare("announce")){
				decode = bencode::type::string::decode(it->second);

				value = std::string(decode.begin(), decode.end());
				list.push_back(value);

				break;
			}
		}
	}

	return list;
}


std::vector<char> TorrentFileProvider::pieces(){
	std::vector<char> value;
	std::string key;

	for(bencode::map::iterator it = info_dictionary.begin(); it != info_dictionary.end(); it++){
		value = bencode::type::string::decode(it->first);
		key = std::string(value.begin(), value.end());

		if(!key.compare("pieces")){
			value = bencode::type::string::decode(it->second);
			break;
		}

	}

	return value;
}

size_t TorrentFileProvider::piece_length(){
	std::vector<char> value;
	std::string key;
	size_t length;

	for(bencode::map::iterator it = info_dictionary.begin(); it != info_dictionary.end(); it++){
		value = bencode::type::string::decode(it->first);
		key = std::string(value.begin(), value.end());

		if(!key.compare("piece length")){
			length = (long)bencode::type::integer::decode(it->second);
			break;
		}
	}

	return length;
}



std::string TorrentFileProvider::size(){
	std::stringstream length;
	std::vector<char> value;
	std::string key;
	int64_t size = 0;
	bencode::list files_list;
	bencode::map file_dictionary;

	for(bencode::map::iterator it = info_dictionary.begin(); it != info_dictionary.end(); it++){
		value = bencode::type::string::decode(it->first);
		key = std::string(value.begin(), value.end());

		if(!key.compare("length")){
			length << bencode::type::integer::decode(it->second);
			break;
		}

		if(!key.compare("files")){
			files_list = bencode::type::list::decode(it->second);

			for(bencode::list::iterator file = files_list.begin(); file != files_list.end(); file++){
				file_dictionary = bencode::type::dictionary::decode(*file);

				for(bencode::map::iterator file_info = file_dictionary.begin(); 
					file_info != file_dictionary.end(); file_info++){

					value = bencode::type::string::decode(file_info->first);
					key = std::string(value.begin(), value.end());
					
					if(!key.compare("length")){
						size += bencode::type::integer::decode(file_info->second);
						break;
					}
				}

			}
			
			length << size;
			break;
		}
	}

	return std::string(length.str());
}

std::string TorrentFileProvider::info_hash(){
	bencode::element* dictionary = 0;
	std::vector<char> key;
	std::string value;

	if(hash.size()){
		return hash;
	}

	for(bencode::map::iterator it = source.begin(); it != source.end(); it++){
		key = bencode::type::string::decode(it->first);
		value = std::string(key.begin(), key.end());

		if(!value.compare("info")){
			dictionary = it->second;
			break;
		}
	}

	bencode::consumer::vector consumer;
	consumer.set(dictionary);

	BlockVectorProvider provider(consumer.vector());
	SecureHashAlgorithm sha(&provider);
	hash = sha.hash();

	return hash;
	return "";
}