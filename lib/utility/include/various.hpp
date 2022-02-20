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

// TODO: create .tpp file

#ifndef U_LIB_VARIOUS_HPP
#define U_LIB_VARIOUS_HPP

#include <utility>
#include <tuple>
#include <random>

#include "../../IO/IO.hpp"
#include "trivial_typedefs.hpp"

namespace u_lib
{
	template <typename Return_Type, typename... Arg_Types>
	using Delegate = Return_Type(*)(Arg_Types...);

	template <typename T>
	concept Indexable = requires(T t, int index)
	{
		t.size();
		t.operator[](index);
	};

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

		// get random item from random accessable container
		template <Indexable T>
		auto& random_item(T& t)
		{
			if (t.size() == 0)
			{
				IO::error("can't pick random item from empty container\n");
			}

			return t[randint(t.size() - 1)];
		}

		// get random item pair from random accessable container
		// item1 != item2
		template <Indexable T>
		auto random_item_pair(T& t)
		{
			using std::declval;
			using std::tuple;
			using inner_type = decltype(declval<T>()[declval<size_t>()]);

			if (t.size() <= 1)
			{
				IO::error("can't pick random item pair from container with less than two elements\n");
			}

			size_t index_1 = randint(t.size() - 1);
			// chose from every - 1 items
			size_t index_2 = randint(t.size() - 2);
			// check if items are same - if yes: choose not in range included item
			if (index_1 == index_2)
			{
				index_2 = t.size() - 1;
			}

			return tuple<inner_type&, inner_type&>(t[index_1], t[index_2]);
		}

		uint get_seed() const;

	private:
		uint _seed;
		std::mt19937 _mt;
	};
}

#endif // !U_LIB_VARIOUS_HPP
