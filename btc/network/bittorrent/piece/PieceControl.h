#pragma once

#include <vector>
#include <map>
#include <stack>
#include <boost/thread.hpp>
#include <boost/interprocess/exceptions.hpp>

#include "TorrentPiece.h"

/*distributes the pieces of the downloadable files
 */

namespace network{
	namespace bittorrent{
		namespace piece{
			typedef class PieceControl{
				private:
                    boost::mutex _guard;                                            //thread safety
					std::vector<char> _pieces;
					size_t _piece_length;

					std::map<int, std::vector<char> > _piece_map;
					std::list<std::pair<long, std::vector<char> > > _shadow_map;

					std::list<std::pair<long, std::vector<char> > >::iterator _next_piece;


				public:
					PieceControl(std::vector<char> pieces, size_t piece_length);
					~PieceControl();

                    void downloaded(TorrentPiece* piece);                           //mark piece as downloaded
                    int piece_length();
                    long left();

                    TorrentPiece* next_piece();                                     //get the new piece
			}control;
		}
	}
}
