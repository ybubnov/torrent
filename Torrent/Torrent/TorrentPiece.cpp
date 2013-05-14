#include "TorrentPiece.h"

TorrentPiece::TorrentPiece(std::vector<char> piece, int index){
	_piece = piece;
	_index = index;
}

TorrentPiece* TorrentPiece::create(std::vector<char> piece, int index){
	return new TorrentPiece(piece, index);
}

int TorrentPiece::index(){
	return _index;
}

std::vector<char> TorrentPiece::piece(){
	return _piece;
}