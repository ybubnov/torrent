#pragma once

#include <ostream>

/*provides an interface to report that data has recived
 */

namespace network{
	typedef class ResponsibleInterface{
		public:
			virtual void response_handle(std::istream&) = 0;
            virtual ~ResponsibleInterface(){}
	}responsible;
}
