#include "BitField.h"

using namespace network::bittorrent::message;

BitField::BitField(std::vector<bool> input){
	long size = input.size();

	data.insert(data.end(), (char)((size & 0xff000000) >> 0x18));
	data.insert(data.end(), (char)((size & 0x00ff0000) >> 0x10));
	data.insert(data.end(), (char)((size & 0x0000ff00) >> 0x8));
	data.insert(data.end(), (char)((size & 0x000000ff)));

	data.insert(data.end(), 0x5);

	throw std::exception(); //not finished
}

BitField BitField::create(std::vector<bool> input){
	return BitField(input);
}

std::vector<char> BitField::message(){
	return data;
}

type BitField::what(){
	return _bitfield;
}

bool BitField::is(std::vector<char>& incoming){
	return incoming.size() > 5 && incoming[4] == 5;
}

std::vector<bool> BitField::field(std::vector<char>& incoming){
	std::vector<bool> bit_field;
	char symbol;
	long length = 0;

	if(incoming.size() < 6){
		throw std::exception();
	}

	length |= (((unsigned long)incoming[0] & 0xff) << 0x18);
	length |= (((unsigned long)incoming[1] & 0xff) << 0x10);
	length |= (((unsigned long)incoming[2] & 0xff) << 0x8);
	length |= (((unsigned long)incoming[3] & 0xff));

	std::vector<char> block(incoming.begin() + 5, incoming.begin() + 5 + length);

	for(std::vector<char>::iterator char_ptr = block.begin(); char_ptr != block.end(); char_ptr++){
		symbol = *char_ptr;

		bit_field.push_back((symbol & 0x80) ? true : false);
		bit_field.push_back((symbol & 0x40) ? true : false);
		bit_field.push_back((symbol & 0x20) ? true : false);
		bit_field.push_back((symbol & 0x10) ? true : false);
		bit_field.push_back((symbol & 0x08) ? true : false);
		bit_field.push_back((symbol & 0x04) ? true : false);
		bit_field.push_back((symbol & 0x02) ? true : false);
		bit_field.push_back((symbol & 0x01) ? true : false);
	}

	return bit_field;
}