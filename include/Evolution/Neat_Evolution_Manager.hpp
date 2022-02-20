// ch 2021
// c++20
// v1.0.0
// vrntzt
// neat
// evolution manager

/* CHANGELOG
 *
 */

/* SUMMARY
 *
 * 
 */

// SPLIT: Evolution Manager - Habitate - Genotype Container (Population) - 
//		  species

// URGENT: either make population the managed ressource or a vector of
//		   pointers - species and champions pointer are VERY dangerous
//		   when pointed object might be copied!

// IMPORTANT: add mechanic to _reset_population to remove stagnant species
// IMPORTANT: check if population size is constant
// IMPORTANT: species won't change as long as stagnant species mechanic isn't
//			  implemented cause champions and their according species are
//			  transfered in every new generation
// IMPORTANT: reconsider usage of shared_ptr for genotype container
//			  most important is reference to a specific individual - both internal
//			  (e.g. via species or champion) and external (when getting population
//			  or single genotype for testing)
//			  internal: could be done with indexing, but might be susceptible to
//			  bugs
//			  external: maybe add fitness-reporting mechanism to genotype and
//			  delegate storing of fitness to neat evolution, this could include
//			  an validity check - should create an
//			  evolution manager and some kind of habitate-class

// TODO: create load & save class (in IO?)
// TODO: delegate interspecies mating chance to Genotype
// TODO: improve debugging and monitoring (e.g. species info)

#ifndef VRNTZT_NEAT_EVOLUTION_MANAGER_HPP
#define VRNTZT_NEAT_EVOLUTION_MANAGER_HPP

#include <string>
#include <vector>

#include <lib/utility/include/trivial_typedefs.hpp>

#include "src/vrntzt_concepts.hpp"

namespace vrntzt::neat
{
	constexpr bool NEAT_EVOLUTION_MANAGER_DEBUG = true;
	constexpr bool NEAT_EVOLUTION_MANAGER_SPECIATION_DEBUG = true;
	constexpr bool NEAT_EVOLUTION_MANAGER_EVOLUTION_DEBUG = true;

	struct Neat_Evolution_Settings
	{
	public:
		int population_size = 100;
		// will split population in batches for processing
		// int batch_size = 100;
		int species_count = 15;
		float interspecies_mating_chance = 0.001f;
	};

	template <Genotype_Type Genotype>
	using Genotype_Container = std::vector<std::unique_ptr<Genotype>>;

	// standard evolution manager
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	class Neat_Evolution_Manager
	{
		// determines how much of the population is allowed to reproduce
		// decision is made based on fitness
		const float PROPAGABLE_FRACTION = 0.4f;

	public:
		explicit Neat_Evolution_Manager(int t_input_num, int t_output_num,
			Neat_Evolution_Settings& t_settings);
		virtual ~Neat_Evolution_Manager();

		Neat_Evolution_Manager(Neat_Evolution_Manager&) = delete;
		Neat_Evolution_Manager& operator=(Neat_Evolution_Manager&) = delete;

		Genotype_Container<Genotype>& get_population();

		void create_random_population();

		void evolve_population();

		// load and save
		// file path can be both relative and absolute
		// void save_population(std::string t_file_path);
		// void load_population(std::string t_file_path);

	private:
		// species with former champions will already exist if not first gen
		void _speciate_population();
		// produce specified number of species offspring - offspring
		// will include current champion!
		Genotype_Container<Genotype> _produce_species_offspring(
			std::vector<Genotype*>& t_species, int t_offspring_num);
		// reset population and species except for champions
		void _reset_population();
		// calculates square average of species
		void _calculate_species_sq_avg_fitness();
		// returns champions of every species
		Genotype* _find_champion(std::vector<Genotype*>& t_genotypes);

		// removes weak individuals; dictated by PROPAGABLE_FRACTION
		void _eliminate_weak_individuals(std::vector<Genotype*>& t_species);

		// distance which needs to be exceeded to be in other species
		// every distance at begin will be 6, cause there won't be 
		// matching genomes (AT THE MOMENT! WILL CHANGE WHEN DOUBLE
		// MUTATIONS GET SAME INNOVATION NUMBER!)
		float _compatibility_threshold = 6.1f;

		// settings
		int _input_num = 0;
		int _output_num = 0;

		int _population_size = 100;
		int _species_count = 15;
		float _interspecies_mating_chance = 0.001f;

		Genotype_Container<Genotype> _population;
		// specified population
		std::vector<std::vector<Genotype*>> _species;
		// number of species (and thereby champions) transfered from previous
		// generation
		int _num_old_species;
		// square average fitness of species
		// the fitness of the individuals when calculating this average
		// is adjusted with respect to the number of genotypes in this
		// species. this has the effect that bigger species need
		// to perform better to grow than smaller ones
		// more specific: average is not sum/n but sum/n^2
		std::vector<float> _species_sq_avg_fitness;
		// average of all species square averages
		float _total_species_fitness = 0.0f;
	};
}

#include "src/Evolution/Neat_Evolution_Manager.tpp"

#endif // !VRNTZT_NEAT_EVOLUTION_MANAGER_HPP
