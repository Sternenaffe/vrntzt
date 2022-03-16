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

// TODO: move Random_Generator to own file
// TODO: integrate u_lib::IO
// TODO: create .tpp file
// TODO: overload Random_Generator methods and add bound checking - maybe with templates
//		 for return type?

#ifndef U_LIB_VARIOUS_HPP
#define U_LIB_VARIOUS_HPP

#include <iostream>
#include <utility>
#include <tuple>
#include <random>
#include <concepts>

#include "trivial_typedefs.hpp"

namespace u_lib
{
	template <typename Return_Type, typename... Arg_Types>
	using Delegate = Return_Type(*)(Arg_Types...);

	template <typename T>
	concept Indexable = requires(T t, int index)
	{
		t.size();
		t[index];
	};

	template <Indexable T>
	struct Inner_Type
	{
		// type which is inside Indexable Ts
		using type = decltype(std::declval<T>()[std::declval<size_t>()]);
	};

	class Random_Generator final
	{
	public:
		// create random generator, will get seed from OS
		explicit Random_Generator();
		// create random generator with custom seed
		explicit Random_Generator(uint t_seed);

		// get random int; 0 <= number <= t_max
		size_t randint(size_t t_max);
		// get random int; t_min <= number <= t_max
		int randint(int t_min, int t_max);

		// float generation; inspired by https://stackoverflow.com/questions/686353/random-float-number-generation
		// get random int; 0 <= number <= 1
		float rand();
		// get random float; 0 <= number <= t_max
		// long double rand(long double t_max);
		float rand(float t_max);
		// get random float; t_min <= number <= t_max
		// long double rand(long double t_min, long double t_max);
		float rand(float t_min, float t_max);


		// returns true if chance has occured
		// t_chance is a probability between 0 and 1
		bool chance(float t_chance);

		// get random item from random accessable container
		template <Indexable T, typename _Inner = Inner_Type<T>::type>
		_Inner& random_item(T& t)
		{
			if (t.size() == 0)
			{
				std::cout << "can't pick random item from empty container\n";
				throw;
			}

			return t[randint(t.size() - 1)];
		}

		// get random item from random accessable container which is different
		// from passed t_item
		template <Indexable T, typename _Inner = Inner_Type<T>::type>
			requires std::equality_comparable<_Inner>
		_Inner& different_random_item(T& t, _Inner& t_item)
		{
			if (t.size() < 2)
			{
				std::cout << "can't pick two distinct random items from container"
					"with less than two items\n";
				throw;
			}

			// chose from every - 1 items (- 2 cause bounds are included)
			size_t other_item_index = randint(t.size() - 2);
			// check if items are same - if yes: choose not in range included item
			if (t[other_item_index] == t_item)
			{
				other_item_index = t.size() - 1;
			}

			return t[other_item_index];
		}

		// get random item pair from random accessable container
		// item1 != item2
		template <Indexable T, typename _Inner = Inner_Type<T>::type>
		std::tuple<_Inner&, _Inner&> random_item_pair(T& t)
		{
			using std::tuple;

			if (t.size() < 2)
			{
				std::cout << "can't pick two distinct random items from container"
					"with less than two items\n";
				throw;
			}

			size_t index_1 = randint(t.size() - 1);
			// chose from every - 1 items
			size_t index_2 = randint(t.size() - 2);
			// check if items are same - if yes: choose not in range included item
			if (index_1 == index_2)
			{
				index_2 = t.size() - 1;
			}

			return tuple<_Inner&, _Inner&>(t[index_1], t[index_2]);
		}

		uint get_seed() const;

	private:
		uint _seed;
		std::mt19937 _mt;
	};
}

#endif // !U_LIB_VARIOUS_HPP
