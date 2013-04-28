#include "Cancel.h"

using namespace network::bittorrent::message;

Cancel::Cancel(long index, long begin, long length){
	data = std::vector<char>(3, 0);

	data.insert(data.end(), 0xd);
	data.insert(data.end(), 0x8);

	data.insert(data.end(), (char)((index & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((index & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((index & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((index & 0x000000ff)));

	data.insert(data.end(), (char)((begin & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((begin & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((begin & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((begin & 0x000000ff)));

	data.insert(data.end(), (char)((length & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((length & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((length & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((length & 0x000000ff)));
}

Cancel Cancel::create(long index, long begin, long length){
	return Cancel(index, begin, length);
}

std::vector<char> Cancel::message(){
	return data;
}

type Cancel::what(){
	return _cancel;
}