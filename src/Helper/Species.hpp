// ch 2021
// c++20
// 1.0.0
// vrntzt
// NEAT Header
// Species class

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_NEAT_SPECIES_HPP
#define VRNTZT_NEAT_SPECIES_HPP

#include <memory>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/vrntzt_global.hpp"
#include "src/vrntzt_concepts.hpp"

namespace vrntzt::neat
{
	constexpr bool VRNTZT_NEAT_SPECIES_DEBUG = false;

	template <Genotype_Type Genotype>
	class Species final
	{
	public:

		explicit Species(Genotype* t_ref_genotype);
		~Species() = default;

		Species(const Species&) = delete;
		Species(const Species&&) = delete;
		Species operator=(const Species&) = delete;
		Species operator=(const Species&&) = delete;

		// returns true if compatible - else false
		bool is_compatible(Genotype* t_genotype, float t_threshold);
		// returns true if compatible - else false
		// if new Genotype has higher fitness than champion -> updates champion
		bool add_if_compatible(Genotype* t_genotype, float t_threshold);

		size_t size();

		Genotype* get_champion();

		constexpr void calculate_metrics();
		// species fitness functions
		constexpr float get_fitness_sum();
		constexpr float get_fitness_sq_avg();

		// std::vector<Genotype*>::Iterator begin();
		// std::vector<Genotype*>::Iterator end();

		// void clear();

	private:
		std::vector<Genotype*> _individuals;
		// champion is also distance reference
		Genotype* _champion = nullptr;

		// metrics
		float _fitness_sum = 0.0f;
		float _fitness_sq_avg = 0.0f;
		// flag which indicates if metrics are calculated
		bool _valid_metrics = false;
	};
}

#endif // !VRNTZT_NEAT_SPECIES_HPP
