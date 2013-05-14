#pragma once

#include <vector>
#include <map>
#include <boost\thread.hpp>

#include "TorrentPiece.h"

class PieceControl{
	private:
		boost::mutex _guard;
		std::vector<char> _pieces;
		size_t _piece_length;

		std::map<int, std::vector<char> > _piece_map;
		std::map<int, std::vector<char> > _shadow_map;

		std::map<int, std::vector<char> >::iterator _next_piece;

	public:
		PieceControl(std::vector<char> pieces, size_t piece_length);
		
		void downloaded(TorrentPiece piece);
		int piece_length();
		TorrentPiece* next_piece();
};