#include "Piece.h"

Piece::Piece(std::vector<char> piece, int index){
	_piece = piece;
	_index = index;
}

Piece Piece::create(std::vector<char> piece, int index){
	return Piece(piece, index);
}

int Piece::index(){
	return _index;
}

std::vector<char> Piece::piece(){
	return _piece;
}