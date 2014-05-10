#pragma once

#include "file_parser.h"
#include "TrackerResponseParser.h"

#include "io.h"
#include "core.h"
#include "peer_wire.h"
#include "../http.h"

#include <vector>
#include <sstream>
#include <list>
#include <stack>

namespace network{
    namespace bittorrent{
        typedef class BitTorrentProtocol : public network::responsible, public network::game_overable{
            private:
                static const char question_mark;                                    //const value of question mark letter
                static const int active_thread;
                static const int finish_thread;

                std::list<network::http::protocol*> httpList;                       //list of the announce http addresses
                std::vector<char> responseData;                                     //http-style response on the http protocol announce request
                std::list<network::bittorrent::peer_wire::peer> peerList;           //list of the active peers
                std::list<network::bittorrent::io::basic_file*> finalFiles;


                network::bittorrent::basic_parser& torrentFile;                       //torrent file wrapper
                network::bittorrent::tracker_response_parser* trackerResponse;      //torrent tracker responce wrapper
                network::bittorrent::io::notifiable* _subscriber;

                network::bittorrent::io::basic_file* loadAdapter;

                boost::thread* trackerRequest;                                      //tracker requests
                boost::thread* peerConversation;                                    //conversations with peers
                boost::mutex _guard;

                long _active_threads;

                std::wstring download_folder;
                std::string peer_id;
                std::vector<char> raw_peer_id;
                std::string key;                                                    //announe request field

                network::bittorrent::piece::control* pieceControl;
                std::list<network::bittorrent::core::executable*> _incoming_threads;

                std::stack<int> threadStack;

                bool _is_interrupted;
                bool _repeated;

            protected:
                void announce_request();
                void execute_downloading();
                void resume_downloading();

            public:
                BitTorrentProtocol(network::bittorrent::io::notifiable* subscriber,
                                   network::bittorrent::basic_parser& file, std::wstring folder);
                ~BitTorrentProtocol();

                bool alive();                                                       //is threads alive

                void game_over();                                                   //do not use this
                void response_handle(std::istream& response);                       //and this

                void interrupt();                                                   //interruption request
                void restart();
                void yeild();                                                       //invoke conversation

                std::wstring folder();                                              //target folder
        }protocol;
    }
}
