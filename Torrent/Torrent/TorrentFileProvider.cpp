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

	std::map<std::string, bencode::element*>::iterator info = map.find("info");
	info_dictionary = bencode::type::dictionary::decode(info->second);
}

std::list<DownloadFile> TorrentFileProvider::files(){
	std::vector<char> value;

	bencode::map bencode_map(info_dictionary.begin(), info_dictionary.end());
	bencode::map file_map;
	bencode::list path_list;
	bencode::list files_list;

	std::string path;
	int64_t length;
	std::string md5sum;

	if(fileList.size()){
		return fileList;
	}


	bencode::map::iterator files_iterator = std::find_if(
		bencode_map.begin(), bencode_map.end(), bencode::string_comparator("files"));
	
	if(files_iterator != bencode_map.end()){
		files_list = bencode::type::list::decode(files_iterator->second);

		for(bencode::list::iterator it = files_list.begin(); it != files_list.end(); it++){
			file_map = bencode::type::dictionary::decode(*it);		
				
			path = std::string();
			md5sum = std::string();
			length = 0;

			bencode::map::iterator path_iterator = std::find_if(
				file_map.begin(), file_map.end(), bencode::string_comparator("path"));

			bencode::map::iterator length_iterator = std::find_if(
				file_map.begin(), file_map.end(), bencode::string_comparator("length"));

			bencode::map::iterator md5sum_iterator = std::find_if(
				file_map.begin(), file_map.end(), bencode::string_comparator("md5sum"));


			if(path_iterator != file_map.end()){
				path_list = bencode::type::list::decode(path_iterator->second);

				for(bencode::list::iterator it = path_list.begin(); it != path_list.end(); it++){
					value = bencode::type::string::decode(*it);
					path += "/";
					path += std::string(value.begin(), value.end());
				}
			}

			if(length_iterator != file_map.end()){
				length = bencode::type::integer::decode(length_iterator->second);
			}

			if(md5sum_iterator != file_map.end()){
				value = bencode::type::string::decode(md5sum_iterator->second);
				path = std::string(value.begin(), value.end());
			}

			fileList.push_back(DownloadFile(length, path, md5sum));
		}

		return fileList;
	}


	bencode::map::iterator name_iterator = std::find_if(
		bencode_map.begin(), bencode_map.end(), 
		bencode::string_comparator("name"));

	bencode::map::iterator length_iterator = std::find_if(
		bencode_map.begin(), bencode_map.end(), 
		bencode::string_comparator("length"));

	bencode::map::iterator md5sum_iterator = std::find_if(
		bencode_map.begin(), bencode_map.end(), 
		bencode::string_comparator("md5sum"));

	if(name_iterator != bencode_map.end()){
		value = bencode::type::string::decode(name_iterator->second);
		path = std::string(value.begin(), value.end());
	}

	if(length_iterator != bencode_map.end()){
		length = bencode::type::integer::decode(length_iterator->second);
	}

	if(md5sum_iterator != bencode_map.end()){
		value = bencode::type::string::decode(md5sum_iterator->second);
		md5sum = std::string(value.begin(), value.end());
	}



	fileList.push_back(DownloadFile(length, path, md5sum));

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

	std::map<std::string, bencode::element*>::iterator announce_iterator = map.find("announce-list");

	if(announce_iterator != map.end()){
		bencode_list = bencode::type::list::decode(announce_iterator->second);

		for(bencode::list::iterator it = bencode_list.begin(); it != bencode_list.end(); it++){
			bencode::list decode_list = bencode::type::list::decode(*it);

			for(bencode::list::iterator it = decode_list.begin(); it != decode_list.end(); it++){
				decode = bencode::type::string::decode(*it);

				value = std::string(decode.begin(), decode.end());
				list.push_back(value);
			}
		}
	}

	if(!bencode_list.size()){
		announce_iterator = map.find("announce");

		if(announce_iterator != map.end()){
			decode = bencode::type::string::decode(announce_iterator->second);

			value = std::string(decode.begin(), decode.end());
			list.push_back(value);
		}
	}

	return list;
}


std::vector<char> TorrentFileProvider::pieces(){
	std::vector<char> value;

	bencode::map::iterator pieces_iterator = std::find_if(
		info_dictionary.begin(), info_dictionary.end(),
		bencode::string_comparator("pieces"));

	if(pieces_iterator != info_dictionary.end()){
		value = bencode::type::string::decode(pieces_iterator->second);
	}else{
		throw std::exception();
	}

	return value;
}

size_t TorrentFileProvider::piece_length(){
	size_t length;

	bencode::map::iterator piece_iterator = std::find_if(
		info_dictionary.begin(), info_dictionary.end(), 
		bencode::string_comparator("piece length"));

	if(piece_iterator != info_dictionary.end()){
		length = (long)bencode::type::integer::decode(piece_iterator->second);
	}else{
		throw std::exception();
	}

	return length;
}



std::string TorrentFileProvider::size(){
	std::stringstream length;
	int64_t size = 0;
	bencode::list files_list;
	bencode::map file_dictionary;

	bencode::map::iterator length_iterator = std::find_if(
		info_dictionary.begin(), info_dictionary.end(), bencode::string_comparator("length"));

	bencode::map::iterator files_iterator = std::find_if(
		info_dictionary.begin(), info_dictionary.end(), bencode::string_comparator("files"));

	if(length_iterator != info_dictionary.end()){
		length << bencode::type::integer::decode(length_iterator->second);
	}else if(files_iterator != info_dictionary.end()){
		files_list = bencode::type::list::decode(files_iterator->second);

		for(bencode::list::iterator file = files_list.begin(); file != files_list.end(); file++){
			file_dictionary = bencode::type::dictionary::decode(*file);

			bencode::map::iterator file_info = std::find_if(
				file_dictionary.begin(), file_dictionary.end(), 
				bencode::string_comparator("length"));

			if(file_info != file_dictionary.end()){
				size += bencode::type::integer::decode(file_info->second);
			}
		}
			
		length << size;
	}

	return std::string(length.str());
}

std::string TorrentFileProvider::info_hash(){
	bencode::element* dictionary = 0;

	if(hash.size()){
		return hash;
	}

	bencode::map::iterator dictionary_iterator = std::find_if(
		source.begin(), source.end(), bencode::string_comparator("info"));

	dictionary = dictionary_iterator->second;

	bencode::consumer::vector consumer;
	consumer.set(dictionary);

	encryption::sha::provider::vector provider(consumer.vector());
	encryption::sha::algorithm sha(&provider);
	hash = sha.hash();

	return hash;
}