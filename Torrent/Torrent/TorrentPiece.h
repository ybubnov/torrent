#pragma once

#include <vector>

class TorrentPiece{
	private:
		std::vector<char> _piece;
		int _index;

		

	public:
		TorrentPiece(std::vector<char> piece, int index);
		static TorrentPiece* create(std::vector<char>, int index);

		int index();
		std::vector<char> piece();
};