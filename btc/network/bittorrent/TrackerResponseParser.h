#pragma once

#include "../../bencode.h"

namespace network{
	namespace bittorrent{
		typedef class TrackerResponseParser{
			private:	
				bencode::map source;
				std::map<std::string, bencode::element*> response;

				std::string _failure_reason;
				std::string _warning_message;
				std::vector<char> _tracker_id;
				std::vector<char> _bpeers;
				long _interval;
				long _min_interval;
				long _complete;
				long _incomplete;

				bool _fr;
				bool _wm;
				bool _ti;
				bool _bp;
				bool _i;
				bool _mi;
				bool _c;
				bool _ic;

			public:
				TrackerResponseParser(bencode::element* dictionary);
				~TrackerResponseParser();

				std::string failure_reason();
				std::string warning_message();
				long interval();
				long min_interval();
				std::vector<char> tracker_id();
				long complete();
				long incomplete();

				std::vector<char> binary_peers();

				bool bad();
		}tracker_response_parser;
	}
}
