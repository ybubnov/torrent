#pragma once

#include <vector>

/*stores the index of the piece and its sha1 hash
 */

namespace network{
	namespace bittorrent{
		namespace piece{
			typedef class TorrentPiece{
				private:
					std::vector<char> _piece;
					int _index;

				public:
					TorrentPiece(std::vector<char> piece, int index);
					static TorrentPiece* create(std::vector<char>, int index);

                    int index();                                                    //get index
                    std::vector<char> piece();                                      //get sha1 hash
			}torrent_piece;
		}
	}
}
