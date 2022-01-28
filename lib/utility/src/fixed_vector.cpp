// ch 2021
// c++14
// v1.0.0
// u_lib
// source for fixed-size vector

#include <vector>

#include "lib/utility/include/fixed_vector.hpp"

namespace u_lib
{
	template <typename T>
	Fixed_Vector<T>::Fixed_Vector(const size_t t_size)
	{
		_internal_vec = std::vector<T>(t_size);
	}

	template <typename T>
	Fixed_Vector<T>::Fixed_Vector(const std::initializer_list<T> t_init_list)
	{
		// forward initializer list in case vector stores rvalue ref
		_internal_vec = std::vector<T>(std::forward<
			const std::initializer_list<T>>(t_init_list));
	}

	template <typename T>
	T& Fixed_Vector<T>::operator[](const size_t t_size)
	{
		return _internal_vec[t_size];
	}

}
