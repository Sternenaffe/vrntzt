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
#include <numeric>
#include <cmath>

#include "lib/IO/IO.hpp"

#include "include/vrntzt_global.hpp"
#include "src/vrntzt_concepts.hpp"

namespace vrntzt::neat
{
	template<Genotype_Type Genotype>
	Species<Genotype>::Species(std::shared_ptr<Genotype> t_representative)
		:
		_representative(t_representative),
		_champion(t_representative),
		_individuals({ t_representative }),
		_valid_metrics(false),
		_fitness_sq_avg(0),
		_fitness_sum(0),
		_stagnating_for(0)
	{
	}

	template<Genotype_Type Genotype>
	Species<Genotype>::Species(const Species& t_other)
		:
		_representative(t_other._representative),
		_champion(t_other._champion),
		_individuals(t_other._individuals),
		_valid_metrics(t_other._valid_metrics),
		_fitness_sq_avg(t_other._fitness_sq_avg),
		_fitness_sum(t_other._fitness_sum),
		_stagnating_for(t_other._stagnating_for)
	{

	}

	template<Genotype_Type Genotype>
	Species<Genotype>& Species<Genotype>::operator=(
		const Species<Genotype>& t_other)
	{
		_representative = t_other._representative;
		_champion		= t_other._champion;
		_individuals	= t_other._individuals;
		_valid_metrics	= t_other._valid_metrics;
		_fitness_sq_avg = t_other._fitness_sq_avg;
		_fitness_sum	= t_other._fitness_sum;
		_stagnating_for = t_other._stagnating_for;

		return *this;
	}

	template<Genotype_Type Genotype>
	bool operator==(const Species<Genotype>& t_first,
		const Species<Genotype>& t_second)
	{
		bool is_same = true;

		// check for all sorts of parameters
		is_same &= (t_first._representative	== t_second._representative);
		is_same &= (t_first._individuals	== t_second._individuals);

		// first two should determine whether is equal - but if some of
		// the following is false: omit warning
		if (is_same &&
			(t_first._champion		!= t_second._champion		||
			t_first._valid_metrics	!= t_second._valid_metrics	||
			t_first._stagnating_for != t_second._stagnating_for ||
			t_first._fitness_sq_avg != t_second._fitness_sq_avg ||
			t_first._fitness_sum	!= t_second._fitness_sum))
		{
			IO::warning("compared species are equal but in different state\n");
		}

		return is_same;
	}

	template<Genotype_Type Genotype>
	bool operator!=(const Species<Genotype>& t_first,
		const Species<Genotype>& t_second)
	{
		return !(t_first == t_second);
	}

	template<Genotype_Type Genotype>
	std::shared_ptr<Genotype>& Species<Genotype>::operator[](
		const size_t t_index)
	{
		if (t_index < _individuals.size())
		{
			return _individuals[t_index];
		}
		else
		{
			IO::error("index outside bounds\n");
			throw;
		}
		
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::is_compatible(std::shared_ptr<Genotype>& t_genotype, float t_threshold)
	{
		if (_representative->distance(*t_genotype) < t_threshold)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::is_compatible(Species& t_other, float t_threshold)
	{
		return is_compatible(t_other->get_champion(), t_threshold);
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::add_if_compatible(std::shared_ptr<Genotype> t_genotype, float t_threshold)
	{
		if (!is_compatible(t_genotype, t_threshold))
		{
			return false;
		}

		// add to species if compatible
		_individuals.push_back(t_genotype);

		if (_champion == nullptr || t_genotype->get_fitness() > _champion->get_fitness())
		{
			_champion = t_genotype;
		}

		// metrics are not longer valid
		_valid_metrics = false;

		return true;
	}

	template<Genotype_Type Genotype>
	void Species<Genotype>::eliminate_weak_individuals()
	{
		using std::shared_ptr;

		// comparison lambda: returns true if first arg is smaller
		auto comp_genotypes = [](const shared_ptr<Genotype>& genotype1,
			const shared_ptr<Genotype>& genotype2) -> bool
		{
			return genotype1->get_fitness() < genotype2->get_fitness();
		};

		std::sort(_individuals.begin(), _individuals.end(), comp_genotypes);

		// remove first '(1 - PROPAGABLE_FRACTION) * total' genotypes
		// which have lowest fitness
		double non_propagable_faction = static_cast<double>(
			1.0 - PROPAGABLE_FRACTION);
		size_t remove_num = static_cast<int>(
			non_propagable_faction *_individuals.size());
		_individuals.erase(_individuals.begin(),
			_individuals.begin() + remove_num);
	}

	template<Genotype_Type Genotype>
	bool Species<Genotype>::is_stagnating()
	{
		if (_stagnating_for > MAX_STAGNATION_PERIOD)
		{
			if constexpr(VRNTZT_NEAT_SPECIES_STAGNATION_DEBUG)
			{ 
				IO::debug("stagnating species\n");
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	template<Genotype_Type Genotype>
	inline size_t Species<Genotype>::size()
	{
		return _individuals.size();
	}

	template<Genotype_Type Genotype>
	std::shared_ptr<Genotype>& Species<Genotype>::get_champion()
	{
		return _champion;
	}

	template<Genotype_Type Genotype>
	std::shared_ptr<Genotype>& Species<Genotype>::get_representative()
	{
		return _representative;
	}

	template<Genotype_Type Genotype>
	std::shared_ptr<Genotype>& Species<Genotype>::get_random_individual()
	{
		return r_gen.random_item(_individuals);
	}

	template<Genotype_Type Genotype>
	std::shared_ptr<Genotype> Species<Genotype>::
		get_different_random_individual(std::shared_ptr<Genotype> t_genotype)
	{
		return r_gen.different_random_item(_individuals, t_genotype);
	}

	template<Genotype_Type Genotype>
	constexpr void Species<Genotype>::calculate_generation_metrics()
	{
		using std::vector;
		
		if (_valid_metrics)
		{
			IO::warning("metrics should only be calculated once per generation\n");
			return;
		}

		float prev_fitness_sum = _fitness_sum;

		// lambda for adding fitness
		auto add_fitness = [](const float& t_total,
			const std::shared_ptr<Genotype> t_next)
			-> float
		{
			float fitness = t_next->get_fitness();

			if (fitness < 0)
			{
				IO::error("error: genotype has negative fitness\n");
			}

			return t_total + fitness;
		};

		// sum: just sum up all individual genome fitness values
		_fitness_sum = std::accumulate(_individuals.begin(), _individuals.end(),
			0.0f, add_fitness);
		// square average: divide by number of individuals squared
		_fitness_sq_avg = _fitness_sum / static_cast<float>(pow(size(), 2));

		_valid_metrics = true;

		// check if fitness is stagnating
		if (prev_fitness_sum >= _fitness_sum)
		{
			++_stagnating_for;
		}
		else
		{
			_stagnating_for = 0;
		}
	}

	template<Genotype_Type Genotype>
	constexpr float Species<Genotype>::_get_fitness_sum()
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
	
	template<Genotype_Type Genotype>
	void Species<Genotype>::reset_for_next_gen()
	{
		_representative = std::move(_champion);
		_champion = nullptr;
		_individuals.clear();
		_valid_metrics = false;
		// _individuals.push_back(_representative);
	}
}	

#endif // !VRNTZT_NEAT_SPECIES_TPP
