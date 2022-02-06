// ch 2021
// c++14
// v1.0.0
// u_lib
// various source

#include <vector>

#include "../include/various.hpp"

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
	int Random_Generator::randint(int t_max)
	{
		if (t_max == 0)
		{
			return 0;
		}

		return _mt() % (t_max + 1);
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

		return (_mt() % (t_max - t_min + 1)) + t_min;
	}

	// get random float; 0 <= number <= 1
	float Random_Generator::rand()
	{
		float r = static_cast<float>(_mt());
		constexpr float divisor = static_cast<float>(std::mt19937::max());
		return r / divisor;
	}

	// get random float; 0 <= number <= t_max
	float Random_Generator::rand(float t_max)
	{
		float r = static_cast<float>(_mt());
		float divisor = static_cast<float>(_mt.max() / t_max);
		return r / divisor;
	}

	// get random float; t_min <= number <= t_max
	float Random_Generator::rand(float t_min, float t_max)
	{
		float r = static_cast<float>(_mt());
		float divisor = static_cast<float>(_mt.max() / (t_max - t_min));
		return t_min + r / divisor;
	}

	uint Random_Generator::get_seed() const
	{
		return _seed;
	}
}
