#include "TorrentPiece.h"

using namespace network::bittorrent::piece;

TorrentPiece::TorrentPiece(std::vector<char> piece, long index){
	_piece = piece;
	_index = index;
}

TorrentPiece* TorrentPiece::create(std::vector<char> piece, long index){
	return new TorrentPiece(piece, index);
}

long TorrentPiece::index(){
	return _index;
}

std::vector<char> TorrentPiece::piece(){
	return _piece;
}
