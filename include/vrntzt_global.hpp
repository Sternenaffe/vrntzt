// ch 2021-22
// c++20
// v1.0.0
// vrntzt
// global

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_GLOBAL_HPP
#define VRNTZT_GLOBAL_HPP

#include <iostream>
#include <vector>

#include "lib/utility/include/various.hpp"
//#include "lib/utility/include/trivial_typedefs.hpp"

namespace vrntzt
{
	// global random generator
	extern u_lib::Random_Generator r_gen;
}

namespace vrntzt::neat
{
	// global debug settings
	constexpr bool RELEASE_BUILD = false;

	using internal_type = float;
}

#endif // !VRNTZT_GLOBAL_HPP
