// ch 2021
// c++14
// v1.0.0
// u_lib
// header for fixed-size vector

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

// TODO: implement range-checks

#ifndef U_LIB_FIXED_VECTOR_HPP
#define U_LIB_FIXED_VECTOR_HPP

#include <vector>

namespace u_lib
{
	template<typename T>
	class Fixed_Vector
	{
	private:
		// wrapped vector obj
		std::vector<T> _internal_vec;

	public:
		Fixed_Vector(const size_t t_size);
		// for uniform initialization
		Fixed_Vector(const std::initializer_list<T> t_init_list);

		inline T& operator[](const size_t t_index);
	};
}

#endif // !U_LIB_FIXED_VECTOR_HPP
