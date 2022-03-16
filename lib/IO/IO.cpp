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

	// forward dec
	void _default_out_stream(string t_string);
	void _default_debug_stream(string t_string);
	void _default_warning_stream(string t_string);
	void _default_error_stream(string t_string);

	static IO_stream_type _out_stream = _default_out_stream;
	static IO_stream_type _debug_stream = _default_debug_stream;
	static IO_stream_type _warning_stream = _default_warning_stream;
	static IO_stream_type _error_stream = _default_error_stream;

	IO_Error out(string t_string)
	{
		_out_stream(t_string);
		return IO_Error::no_error;
	}

	IO_Error debug(string t_string)
	{
		_out_stream(t_string);
		return IO_Error::no_error;
	}

	IO_Error warning(string t_string)
	{
		_out_stream("Warning: " + t_string);
		return IO_Error::no_error;
	}

	IO_Error error(string t_string)
	{
		_out_stream("Error: " + t_string);
		throw;
		return IO_Error::no_error;
	}

	void set_all_streams(IO_stream_type t_stream)
	{
		set_out_stream(t_stream);
		set_debug_stream(t_stream);
		set_warning_stream(t_stream);
		set_error_stream(t_stream);
	}

	void set_out_stream(IO_stream_type t_stream)
	{
		_out_stream = t_stream;
	}

	void set_debug_stream(IO_stream_type t_stream)
	{
		_out_stream = t_stream;
	}

	void set_warning_stream(IO_stream_type t_stream)
	{
		_out_stream = t_stream;
	}

	void set_error_stream(IO_stream_type t_stream)
	{
		_out_stream = t_stream;
	}

	void _default_out_stream(string t_string)
	{
		std::cout << t_string;
	}

	void _default_debug_stream(string t_string)
	{
		std::cout << t_string;
	}

	void _default_warning_stream(string t_string)
	{
		std::cout << "Warning: " << t_string;
	}

	void _default_error_stream(string t_string)
	{
		std::cout << "Error: " << t_string;
	}
}
