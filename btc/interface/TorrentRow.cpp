#include "TorrentRow.h"

#include <climits>
#include <cstdlib>

using namespace interface::utils;

TorrentRow::TorrentRow(std::string info_hash, std::string file_name, std::string file_size, int status){
    _file_name = file_name;
    _file_size = file_size;
    _status = status;

    _info_hash = info_hash;
}

std::string TorrentRow::get_file_name(){
    return _file_name;
}

std::string TorrentRow::get_file_size(){
    return _file_size;
}

std::string TorrentRow::get_hash(){
    return _info_hash;
}

int TorrentRow::get_status(){
    return _status;
}
