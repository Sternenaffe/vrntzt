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

#ifndef IO_HPP
#define IO_HPP

#include <string>

namespace IO
{
	enum class IO_Error
	{
		no_error
	};

	IO_Error out(std::string t_string);
	IO_Error debug(std::string t_string);
	IO_Error warning(std::string t_string);
	IO_Error error(std::string t_string);
}

#endif // !IO_HPP
