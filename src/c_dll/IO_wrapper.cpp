// ch 2021-22
// c
// v1.0.0
// u_lib
// IO wrapper for a c dll

#include "include/vrntzt_pch.h"
#define VRNTZT_NEAT_C_DLL_EXPORT
#include "include/c_dll/IO_wrapper.h"

#include <string>
#include <string.h>

#include "lib/IO/IO.hpp"

namespace u_lib::IO::c_dll
{
	// forward dec
	void _IO_out_callback(std::string t_string);
	void _IO_debug_callback(std::string t_string);
	void _IO_warning_callback(std::string t_string);
	void _IO_error_callback(std::string t_string);

	static IO_c_callback _out_callback = nullptr;
	static IO_c_callback _debug_callback = nullptr;
	static IO_c_callback _warning_callback = nullptr;
	static IO_c_callback _error_callback = nullptr;

	void set_all_streams(IO_c_callback t_callback_f)
	{
		set_out_stream(t_callback_f);
		set_debug_stream(t_callback_f);
		set_warning_stream(t_callback_f);
		set_error_stream(t_callback_f);
	}

	void set_out_stream(IO_c_callback t_callback_f)
	{
		_out_callback = t_callback_f;
		::IO::set_out_stream(_IO_out_callback);
	}

	void set_debug_stream(IO_c_callback t_callback_f)
	{
		_debug_callback = t_callback_f;
		::IO::set_debug_stream(_IO_debug_callback);
	}

	void set_warning_stream(IO_c_callback t_callback_f)
	{
		_warning_callback = t_callback_f;
		::IO::set_warning_stream(_IO_warning_callback);
	}

	void set_error_stream(IO_c_callback t_callback_f)
	{
		_error_callback = t_callback_f;
		::IO::set_error_stream(_IO_error_callback);
	}

	// convert string to char array in order to call callback f
	void _IO_out_callback(std::string t_string)
	{
		size_t len = t_string.length() + 1;
		char* cstr = new char[len];
		strcpy_s(cstr, len, t_string.c_str());

		_out_callback(cstr, len);

		delete[] cstr;
	}

	// convert string to char array in order to call callback f
	void _IO_debug_callback(std::string t_string)
	{
		size_t len = t_string.length() + 1;
		char* cstr = new char[len];
		strcpy_s(cstr, len, t_string.c_str());

		_debug_callback(cstr, len);

		delete[] cstr;
	}

	// convert string to char array in order to call callback f
	void _IO_warning_callback(std::string t_string)
	{
		size_t len = t_string.length() + 1;
		char* cstr = new char[len];
		strcpy_s(cstr, len, t_string.c_str());

		_warning_callback(cstr, len);

		delete[] cstr;
	}

	// convert string to char array in order to call callback f
	void _IO_error_callback(std::string t_string)
	{
		size_t len = t_string.length() + 1;
		char* cstr = new char[len];
		strcpy_s(cstr, len, t_string.c_str());

		_error_callback(cstr, len);

		delete[] cstr;
	}
}

