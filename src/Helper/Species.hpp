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

// TODO: include species ID
// TODO: create move constructor and assignment operator
// TODO: add reset flag

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
	constexpr bool VRNTZT_NEAT_SPECIES_STAGNATION_DEBUG = true;

	template <Genotype_Type Genotype>
	using Genotype_Container = std::vector<std::shared_ptr<Genotype>>;

	template <Genotype_Type Genotype>
	class Species final
	{
		const int MAX_STAGNATION_PERIOD = 15;
		// determines how much of the population is allowed to reproduce
		// decision is made based on fitness
		const float PROPAGABLE_FRACTION = 0.4f;

	public:
		explicit Species(std::shared_ptr<Genotype> t_representative);
		~Species() = default;

		Species(const Species& t_other);
		Species<Genotype>& operator=(const Species<Genotype>& t_other);

		template <Genotype_Type Genotype>
		friend bool operator==(const Species<Genotype>& t_first,
			const Species<Genotype>& t_second);

		template <Genotype_Type Genotype>
		friend bool operator!=(const Species<Genotype>& t_first,
			const Species<Genotype>& t_second);

		std::shared_ptr<Genotype>& operator[](const size_t t_index);

		// returns true if compatible - else false
		bool is_compatible(std::shared_ptr<Genotype>& t_genotype,
			float t_threshold);
		bool is_compatible(Species<Genotype>& t_other, float t_threshold);
		// returns true if compatible - else false
		// if new Genotype has higher fitness than champion -> updates champion
		// important: copy Genotype ptr, not reference!
		bool add_if_compatible(std::shared_ptr<Genotype> t_genotype,
			float t_threshold);

		// no need to recalculate metrics after call to this function
		void eliminate_weak_individuals();

		// returns true if species has not experienced fitness improvement for
		// too long
		bool is_stagnating() const;

		size_t size() const;

		std::shared_ptr<Genotype>& get_champion();
		std::shared_ptr<Genotype>& get_representative();

		std::shared_ptr<Genotype>& get_random_individual();
		// get a random individual which is different from the passed one
		std::shared_ptr<Genotype> get_different_random_individual(
			std::shared_ptr<Genotype> t_genotype);

		// calculate species metrics
		// call only once per generation!
		constexpr void calculate_generation_metrics();

		// square average fitness of species
		// the fitness of the individuals when calculating this average
		// is adjusted with respect to the number of genotypes in this
		// species. this has the effect that bigger species need
		// to perform better to grow than smaller ones
		// more specific: average is not sum/n but sum/n^2
		constexpr float get_fitness_sq_avg();

		// std::vector<Genotype*>::Iterator begin();
		// std::vector<Genotype*>::Iterator end();

		// resets species and uses current champion as new representative
		void reset_for_next_gen();

	private:
		// species fitness functions
		// sum needed?
		constexpr float _get_fitness_sum();

		Genotype_Container<Genotype> _individuals;
		// distance reference
		std::shared_ptr<Genotype> _representative = nullptr;
		std::shared_ptr<Genotype> _champion = nullptr;

		// metrics
		float _fitness_sum = 0.0f;
		float _fitness_sq_avg = 0.0f;
		// flag which indicates if metrics are calculated
		bool _valid_metrics = false;

		// 
		int _stagnating_for = 0;
	};
}

#include "Species.tpp"

#endif // !VRNTZT_NEAT_SPECIES_HPP
