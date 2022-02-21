// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT
// Species class

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_NEAT_SPECIES_TPP
#define VRNTZT_NEAT_SPECIES_TPP

#include "Species.hpp"

#include <memory>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/vrntzt_global.hpp"
#include "src/vrntzt_concepts.hpp"

namespace vrntzt::neat
{
	template<Genotype_Type Genotype>
	Species<Genotype>::Species(Genotype* t_ref_genotype)
	{
		_individuals.push_back(t_ref_genotype);
		_champion = t_ref_genotype;
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::is_compatible(Genotype* t_genotype, float t_threshold)
	{
		if (_champion->distance(*t_genotype) < t_threshold)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::add_if_compatible(Genotype* t_genotype, float t_threshold)
	{
		if (!is_compatible(t_genotype, t_threshold))
		{
			return false;
		}

		// add to species if compatible
		_individuals.push_back(t_genotype);

		if (t_genotype->get_fitness() > _champion->get_fitness())
		{
			_champion = t_genotype;
		}

		return false;
	}

	template<Genotype_Type Genotype>
	inline size_t Species<Genotype>::size()
	{
		return _individuals.size();
	}

	template<Genotype_Type Genotype>
	Genotype* Species<Genotype>::get_champion()
	{
		return _champion;
	}

	template<Genotype_Type Genotype>
	constexpr void Species<Genotype>::calculate_metrics()
	{
		using std::vector;

		// lambda for adding fitness
		float add_fitness = [](const float& t_total,
			const Genotype* t_next)
		{
			float fitness = t_next->get_fitness();

			if (fitness < 0)
			{
				IO::error("error: genotype has negative fitness\n");
			}

			return t_total + fitness;
		};

		// sum: just sum up all individual genome fitness values
		_fitness_sum = std::accumulate(species.begin(), species.end(),
			0.0f, add_fitness);
		// square average: divide by number of individuals squared
		_fitness_sq_avg = _fitness_sum / pow(size(), 2);
	}

	template<Genotype_Type Genotype>
	constexpr float Species<Genotype>::get_fitness_sum()
	{
		if (!_valid_metrics)
		{
			IO::error("metrics need to be calculated first\n");
			return 0.0;
		}
		else
		{
			return _fitness_sum;
		}
	}

	template<Genotype_Type Genotype>
	constexpr float Species<Genotype>::get_fitness_sq_avg()
	{
		if (!_valid_metrics)
		{
			IO::error("metrics need to be calculated first\n");
			return 0.0;
		}
		else
		{
			return _fitness_sq_avg;
		}
	}
}	

#endif // !VRNTZT_NEAT_SPECIES_TPP
