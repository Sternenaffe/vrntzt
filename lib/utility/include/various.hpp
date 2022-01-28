// ch 2021
// c++14
// v1.0.0
// u_lib
// various helper functions, classes & macros

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef U_LIB_VARIOUS_HPP
#define U_LIB_VARIOUS_HPP

#include <random>

#include "lib/utility/include/trivial_typedefs.hpp"

namespace u_lib
{
	template <typename Return_Type, typename... Arg_Types>
	using Delegate = Return_Type(*)(Arg_Types...);

	class Random_Generator final
	{
	public:
		// create random generator, will get seed from OS
		explicit Random_Generator();
		// create random generator with custom seed
		explicit Random_Generator(uint t_seed);

		// get random int; 0 <= number <= t_max
		int randint(int t_max);
		// get random int; t_min <= number <= t_max
		int randint(int t_min, int t_max);

		// float generation; inspired by https://stackoverflow.com/questions/686353/random-float-number-generation
		// get random int; 0 <= number <= 1
		float rand();
		// get random float; 0 <= number <= t_max
		float rand(float t_max);
		// get random float; t_min <= number <= t_max
		float rand(float t_min, float t_max);

		uint get_seed() const;

	private:
		uint _seed;
		std::mt19937 _mt;
	};
}

#endif // !U_LIB_VARIOUS_HPP
