// ch 2021
// c++14
// v1.0.0
// miscellaneous utility
// IO header

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * defines IO interfaces with easy-to-change output streams for reusable code
  */

// IMPORTANT: switch to streams

// TODO: move into u_lib namespace

#ifndef IO_HPP
#define IO_HPP

#include <string>

#include "../utility/include/various.hpp"

namespace IO
{
	using IO_stream_type = u_lib::Delegate<void, std::string>;

	enum class IO_Error
	{
		no_error
	};

	IO_Error out(std::string t_string);
	IO_Error debug(std::string t_string);
	IO_Error warning(std::string t_string);
	IO_Error error(std::string t_string);

	void set_all_streams(IO_stream_type t_stream);
	void set_out_stream(IO_stream_type t_stream);
	void set_debug_stream(IO_stream_type t_stream);
	void set_warning_stream(IO_stream_type t_stream);
	void set_error_stream(IO_stream_type t_stream);

	/*template <typename T>
	void save_as_xml(T t_obj);

	template <typename T>
	void load_from_xml(T t_obj);*/
}

#include "IO.tpp"

#endif // !IO_HPP
