#pragma once

#include <ostream>

namespace network{
	typedef class ResponsibleInterface{
		public:
			virtual void response_handle(std::istream*) = 0;
	}responsible;
}