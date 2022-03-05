// ch 2021
// c++14
// v1.0.0
// u_lib
// various source

#include <vector>

#include "../include/various.hpp"
#include "../../IO/IO.hpp"

namespace u_lib
{
	// create random generator, will get seed from OS
	Random_Generator::Random_Generator()
		:
		_mt(std::mt19937())
	{
		std::random_device rd;

		_seed = rd();
		_mt.seed(_seed);
	}

	// create random generator with custom seed
	Random_Generator::Random_Generator(uint t_seed)
		:
		_seed(t_seed),
		_mt(std::mt19937(t_seed))
	{
	}

	// get random int; 0 <= number <= t_max
	size_t Random_Generator::randint(size_t t_max)
	{
		if (t_max == 0)
		{
			return 0;
		}

		int r = static_cast<int>(_mt());
		return r % (t_max + 1);
	}

	// get random int; t_min <= number <= t_max
	int Random_Generator::randint(int t_min, int t_max)
	{
		if (t_max == t_min)
		{
			return t_min;
		}
		if (t_max < t_min)
		{
			throw;
		}

		uint r = _mt();
		int modulo_r = static_cast<int>(r % (t_max - t_min + 1));
		return modulo_r + t_min;
	}

	// get random float; 0 <= number <= t_max
	//long double Random_Generator::rand(long double t_max)
	//{
	//	long double r = static_cast<long double>(_mt());
	//	long double divisor = _mt.max() / t_max;
	//	return r / divisor;
	//}

	// get random float; 0 <= number <= 1
	float Random_Generator::rand()
	{
		float r = static_cast<float>(_mt());
		constexpr float divisor = static_cast<float>(std::mt19937::max());
		return r / divisor;
	}

	// get random float; t_min <= number <= t_max
	float Random_Generator::rand(float t_min, float t_max)
	{
		float r = static_cast<float>(_mt());
		float divisor = static_cast<float>(_mt.max() / (t_max - t_min));
		return t_min + r / divisor;
	}

	bool Random_Generator::chance(float t_chance)
	{
		return rand() <= t_chance;
	}

	uint Random_Generator::get_seed() const
	{
		return _seed;
	}
}
