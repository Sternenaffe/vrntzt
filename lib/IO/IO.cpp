// ch 2021
// c++14
// v1.0.0
// miscellaneous utility
// IO header

#include <iostream>
#include <cstdarg>

#include "IO.hpp"

namespace IO
{
	using std::string;

	IO_Error out(string t_string)
	{
		return IO_Error::no_error;
	}

	IO_Error debug(string t_string)
	{
		std::cout << t_string;
		return IO_Error::no_error;
	}

	IO_Error warning(string t_string)
	{
		std::cout << "Warning: " << t_string;
		return IO_Error::no_error;
	}

	IO_Error error(string t_string)
	{
		std::cout << "Error: " << t_string;
		throw;
		return IO_Error::no_error;
	}
}