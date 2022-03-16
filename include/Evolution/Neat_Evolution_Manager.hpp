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

// URGENT: create definition for move constructor & move assignment operator

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

// TODO: check and rethink types
// TODO: rewrite save & load (SRT and HR_Serializable interface)
// TODO: delegate interspecies mating chance to Genotype
// TODO: create more sophisticated algorithm to adjust compatibility threshold
// TODO: improve debugging and monitoring (e.g. species info)
// TODO: check if function args can be made const
// TODO: store in xml file

#ifndef VRNTZT_NEAT_EVOLUTION_MANAGER_HPP
#define VRNTZT_NEAT_EVOLUTION_MANAGER_HPP

#include <string>
#include <vector>

#include "lib/pugixml-1.12/src/pugixml.hpp"
#include "lib/utility/include/trivial_typedefs.hpp"

#include "src/vrntzt_concepts.hpp"
#include "src/Helper/Species.hpp"

namespace vrntzt::neat
{
	constexpr bool NEAT_EVOLUTION_MANAGER_DEBUG = true;
	constexpr bool NEAT_EVOLUTION_MANAGER_SPECIATION_DEBUG = false;
	constexpr bool NEAT_EVOLUTION_MANAGER_EVOLUTION_DEBUG = false;
	constexpr bool NEAT_EVOLUTION_MANAGER_MATING_DEBUG = false;

	// if add constructor: need to write wrapper for c_dll!
	struct Neat_Evolution_Settings
	{
	public:
		int population_size = 100;
		// will split population in batches for processing
		// int batch_size = 100;
		int species_count = 10;
		float interspecies_mating_chance = 0.001f;
	};

	template <Genotype_Type Genotype>
	using Genotype_Container = std::vector<std::shared_ptr<Genotype>>;

	// standard evolution manager
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	class Neat_Evolution_Manager
	{
	private:
		class XML_Handler;

	public:
		explicit Neat_Evolution_Manager(int t_input_num, int t_output_num,
			Neat_Evolution_Settings& t_settings);
		virtual ~Neat_Evolution_Manager();

		Neat_Evolution_Manager(Neat_Evolution_Manager&) = delete;
		Neat_Evolution_Manager(Neat_Evolution_Manager&&) = default;
		Neat_Evolution_Manager& operator=(Neat_Evolution_Manager&) = delete;
		Neat_Evolution_Manager& operator=(Neat_Evolution_Manager&&) = default;

		size_t get_population_size();

		Genotype_Container<Genotype>& get_population();
		// returns best genotype from previous generation
		const std::shared_ptr<Genotype> get_previous_best_genotype();

		void create_random_population();
		void delete_population();

		void evolve_population();

		// load and save
		void save(std::string t_file_path);
		void load(std::string t_file_path);

		// xml load and save
		void save_population_to_xml(pugi::xml_node t_xml_node);
		void load_population_from_xml(pugi::xml_node t_xml_node);

	private:
		void _add_genotype_to_population(const Genotype& t_genotype);

		// evaluate which species should be transfered to next generation, this
		// is in order to track species over multiple generations
		void _eliminate_weak_species();

		// species with former champions will already exist if not first gen
		void _speciate_population();
		
		// delete species which didn't made progress
		// for too long
		// also remove empty species
		void _eliminate_stagnating_species();

		// returns all species champions
		Genotype_Container<Genotype> _get_champions();

		// calculates how many offsprings should be contributed by given species
		// always rounds down cause rounding doesnt ensures that all offspring
		// sum == population size and if rounding down its at least never bigger
		size_t  _calculate_offspring_num(size_t t_total_offspring_num,
			Species<Genotype>& t_species);
		
		// produce specified number of species offspring
		// adds offspring directly to population
		void _produce_species_offspring(Species<Genotype>& t_species,
			size_t t_offspring_num);

		// mates given first parent with random individual from different species
		// and returns this new offspring
		std::shared_ptr<Genotype> _interspecies_reproduction(
			std::shared_ptr<Genotype>& t_first_parent,
			Species<Genotype>& t_first_parent_species);

		// mates given first parent with random individual from same species
		// and returns this new offspring
		std::shared_ptr<Genotype> _sexual_reproduction(
			std::shared_ptr<Genotype>& t_first_parent,
			Species<Genotype>& t_first_parent_species);
		
		// mutates given genotype and returns copy
		std::shared_ptr<Genotype> _asexual_reproduction(
			std::shared_ptr<Genotype>& t_first_parent);

		// distance which needs to be exceeded to be in other species
		// every distance at begin will be 6, cause there won't be 
		// matching genomes (AT THE MOMENT! WILL CHANGE WHEN DOUBLE
		// MUTATIONS GET SAME INNOVATION NUMBER!)
		float _compatibility_threshold = 5.5f;

		// settings
		uint _input_num = 0;
		uint _output_num = 0;

		size_t _population_size = 100;
		size_t _species_count = 15;
		float _interspecies_mating_chance = 0.001f;

		Genotype_Container<Genotype> _population;
		// specified population
		std::vector<Species<Genotype>> _species;

		std::shared_ptr<Genotype> _prev_best_genotype;
		
		// average of all species square averages
		float _total_species_fitness = 0.0f;
	};
}

#include "src/Evolution/Neat_Evolution_Manager.tpp"

#endif // !VRNTZT_NEAT_EVOLUTION_MANAGER_HPP
