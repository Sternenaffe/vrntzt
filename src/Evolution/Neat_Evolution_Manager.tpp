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

#ifndef VRNTZT_NEAT_EVOLUTION_MANAGER_TPP
#define VRNTZT_NEAT_EVOLUTION_MANAGER_TPP

#include "include/vrntzt_pch.h"
#include "include/Evolution/Neat_Evolution_Manager.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
// #include <cmath>

#include "lib/IO/IO.hpp"
#include "lib/utility/include/trivial_typedefs.hpp"
#include "lib/pugixml-1.12/src/pugixml.hpp"

#include "include/vrntzt_global.hpp"
#include "include/Genotype/Simplistic_Genotype.hpp"

namespace vrntzt::neat
{
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	inline Neat_Evolution_Manager<Genotype, Phenotype>::Neat_Evolution_Manager(
		int t_input_num, int t_output_num, 
		Neat_Evolution_Settings& t_settings)
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("create evolution manager\n");
		}

		if (t_input_num <= 0)
		{
			IO::error("input number can't be zero or lower\n");
		}
		if (t_output_num <= 0)
		{
			IO::error("input number can't be zero or lower\n");
		}

		_input_num = t_input_num;
		_output_num = t_output_num;
		_population_size = t_settings.population_size;
		_species_count = t_settings.species_count;
		_interspecies_mating_chance = t_settings.interspecies_mating_chance;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Neat_Evolution_Manager<Genotype, Phenotype>::~Neat_Evolution_Manager()
	{
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	size_t Neat_Evolution_Manager<Genotype, Phenotype>::get_population_size()
	{
		return _population_size;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Genotype_Container<Genotype>& Neat_Evolution_Manager<Genotype, Phenotype>::
		get_population()
	{
		return _population;
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	const std::shared_ptr<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		get_previous_best_genotype()
	{
		return _prev_best_genotype;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::
		create_random_population()
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("creating random population\n");
		}

		reset();
		
		for (uint i = 0; i < _population_size; ++i)
		{
			std::shared_ptr<Genotype> tmp = std::make_shared<Genotype>(
				_input_num, _output_num);
			_population.push_back(std::move(tmp));
		}

		// set best genotype to avoid nullptr
		_prev_best_genotype = *_population.begin();
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::reset()
	{
		_population.clear();
		_species.clear();
		_prev_best_genotype = nullptr;

		_compatibility_threshold = 5.5f;

		// should change!
		Genome::reset_innovation_num();
		Simplistic_Genotype::reset_global_neuron_num();
	}

	// function is not written in the most efficient way - could merge multiple
	// loops; instead the evolution steps are as explicit as possible
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::evolve_population()
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_MATING_DEBUG)
		{
			IO::debug("old population size: " + std::to_string(_population.size()) +
				"\n");
		}

		_calculate_generation_metrics();

		// cleanup old gen
		// ---------------------------- SPLIT -------------------------------
		// propagable species

		// evaluate which species should be transfered to next generation, this
		// is in order to track species over multiple generations
		_eliminate_weak_species();

		// returns elite genotypes which should be inherited unmodified
		Genotype_Container<Genotype> elite = _get_champions();

		_speciate_population();

		// calculate species fitness
		for (Species<Genotype>& species : _species)
		{
			species.calculate_generation_metrics();
		}

		// remove species if their fitness is stagnating (obviously only
		// possible if not a new species)
		_eliminate_stagnating_species();

		// propagable species
		// ---------------------------- SPLIT -------------------------------
		// propagable individuals

		_population.clear();

		_total_species_fitness = 0.0f;
		// prepare species for mating
		for (Species<Genotype>& species : _species)
		{
			_total_species_fitness += species.get_fitness_sq_avg();
			// don't move to _produce_species_offspring because interspecies
			// mating needs every species to be ready for mating
			species.eliminate_weak_individuals();
		}
		
		// propagable individuals
		// ------------------------- SPLIT ---------------------------
		// creating offspring

		size_t total_offspring_num = _population_size - elite.size();

		// mating
		for (Species<Genotype>& species : _species)
		{
			size_t offspring_num = _calculate_offspring_num(total_offspring_num,
				species);
			// adds offspring directly to population
			_produce_species_offspring(species, offspring_num);
		}

		if (total_offspring_num < _population.size())
		{
			IO::error("Fatal Error: population bigger than offspring num\n");
		}

		size_t missing_individuals = total_offspring_num - _population.size();

		if constexpr (NEAT_EVOLUTION_MANAGER_MATING_DEBUG)
		{
			IO::debug("generating " + std::to_string(missing_individuals) + " missing individuals\n");
		}

		// _calculate_offspring_num might generate to few individuals -> fill
		// population with random interspecies mated individuals
		for (uint i = 0; i < missing_individuals; ++i)
		{
			// get random individual from random species and perform interspecies
			// mating
			Species<Genotype>& random_species = r_gen.random_item(_species);
			std::shared_ptr<Genotype>& first_parent = r_gen.random_item(
				random_species);

			// if (_species.size() > 1) - dont need to check cause if there was
			// only one species the offspring number has to be correct
			_population.push_back(_interspecies_reproduction(first_parent,
				random_species));
		}

		// add elite to population
		for (std::shared_ptr<Genotype>& genotype : elite)
		{
			_population.push_back(std::move(genotype));
		}

		// adjust compatibility threshold (unelegant)
		if (_species.size() < _species_count)
		{
			float random_num = r_gen.rand(0.95f, 0.99f);
			// IO::debug("random num: " + std::to_string(random_num) + "\n");
			_compatibility_threshold *= random_num;
		}
		else if (_species.size() > _species_count)
		{
			float random_num = r_gen.rand(1.01f, 1.05f);
			// IO::debug("random num: " + std::to_string(random_num) + "\n");
			_compatibility_threshold *= random_num;
		}

		// change through multiplication: should never get 0 but to make sure:
		_compatibility_threshold = std::max(_compatibility_threshold, 0.1f);

		return;
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::save(std::string t_file_path)
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("save population to " + t_file_path);
		}

		pugi::xml_document doc;
		// append base node
		pugi::xml_node base_node = doc.append_child("Neat_Evolution_Manager");

		save_population_to_xml(base_node);

		doc.save_file(t_file_path.c_str());
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::load(std::string t_file_path)
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("load population from " + t_file_path);
		}

		pugi::xml_document doc;
		doc.load_file(t_file_path.c_str());

		pugi::xml_node neat_evolution_manager_node = doc.document_element();
		if (std::strcmp(neat_evolution_manager_node.name(), "Neat_Evolution_Manager"))
		{
			IO::error("loading file is not valid\n");
		}

		reset();

		// append population node
		pugi::xml_node population_node = neat_evolution_manager_node.first_child();
		load_population_from_xml(population_node);
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::save_population_to_xml(
		pugi::xml_node t_xml_node)
	{
		// append population node
		pugi::xml_node population_node = t_xml_node.append_child("Population");

		// add genotypes
		for (auto& genotype : _population)
		{
			genotype->save_to_xml(population_node);
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::load_population_from_xml(
		pugi::xml_node t_population_node)
	{
		if (std::strcmp(t_population_node.name(), "Population"))
		{
			IO::error("loading file is not valid\n");
		}

		// load genotypes
		for (pugi::xml_node genotype_node = t_population_node.first_child();
			genotype_node; genotype_node = genotype_node.next_sibling())
		{
			Genotype genotype(_input_num, _output_num);
			genotype.load_from_xml(genotype_node);
			_add_genotype_to_population(std::move(genotype));
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_add_genotype_to_population(
		const Genotype& t_genotype)
	{
		std::shared_ptr<Genotype> genotype_ptr = std::make_shared<Genotype>(t_genotype);
		_population.push_back(genotype_ptr);
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_calculate_generation_metrics()
	{
		// get best genotype
		// fitness comparison lambda
		auto comp_fitness = [](const std::shared_ptr<Genotype>& t_first,
			const std::shared_ptr<Genotype>& t_second) -> bool
		{
			// return true if second has larger fitness
			return (t_first->get_fitness() < t_second->get_fitness()) ? true : false;
		};
		// find best genotype of old generation
		_prev_best_genotype = *std::max_element(_population.begin(), _population.end(),
			comp_fitness);

		// calculate most important reproduction setting metrics
		float weight_mutation_chance_avg = 0;
		float sexual_reproduction_chance_avg = 0;
		float add_neuron_chance_avg = 0;
		float add_connection_chance_avg = 0;
		float delete_connection_chance_avg = 0;

		for (auto genotype : _population)
		{
			sexual_reproduction_chance_avg += genotype->_sexual_reproduction_chance;
			add_neuron_chance_avg += genotype->_add_neuron_chance;
			add_connection_chance_avg += genotype->_add_connection_chance;
			delete_connection_chance_avg += genotype->_delete_connection_chance;
			weight_mutation_chance_avg += genotype->_weight_mutation_chance;
		}

		sexual_reproduction_chance_avg /= _population.size();
		add_neuron_chance_avg /= _population.size();
		add_connection_chance_avg /= _population.size();
		delete_connection_chance_avg /= _population.size();
		weight_mutation_chance_avg /= _population.size();

		if (NEAT_EVOLUTION_MANAGER_REPRODUCTION_SETTINGS_DEBUG)
		{
			IO::debug("sexual_reproduction_chance_avg: " + std::to_string(sexual_reproduction_chance_avg) + "\n");
			IO::debug("add_neuron_chance_avg: " + std::to_string(add_neuron_chance_avg) + "\n");
			IO::debug("add_connection_chance_avg: " + std::to_string(add_connection_chance_avg) + "\n");
			IO::debug("delete_connection_chance_avg: " + std::to_string(delete_connection_chance_avg) + "\n");
			IO::debug("weight_mutation_chance_avg: " + std::to_string(weight_mutation_chance_avg) + "\n");
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_eliminate_weak_species()
	{
		typename std::vector<Species<Genotype>> new_species;

		for (Species<Genotype>& species : _species)
		{
			if (species.size() >= _population_size / _species_count)
			{
				new_species.push_back(species);
			}
		}

		_species = new_species;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_speciate_population()
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("speciate population\n");
		}

		// reset species
		for (Species<Genotype>& species : _species)
		{
			species.reset_for_next_gen();
		}

		// loop through whole population and create new species if 
		// matches no existing species
		for (std::shared_ptr<Genotype>& genotype : _population)
		{
			bool added_to_species = false;

			for (Species<Genotype>& species : _species)
			{
				added_to_species = species.add_if_compatible(
					genotype, _compatibility_threshold);

				if (added_to_species)
				{
					break;
				}
			}

			if (!added_to_species)
			{
				_species.push_back(Species<Genotype>(genotype));
			}
		}

		// remove empty species (might be inherited by prev generation but remained
		// unfilled)
		_species.erase(std::remove_if(_species.begin(), _species.end(),
			[](auto const& t_e) { return t_e.size() == 0; }),
			_species.end());

		if constexpr (NEAT_EVOLUTION_MANAGER_SPECIATION_DEBUG)
		{
			using std::to_string;

			IO::debug("\nSpecies: \n");
			IO::debug("compatibility_threshold: " +
				std::to_string(_compatibility_threshold) + "\n");

			size_t species_num = 0;
			for (auto& species : _species)
			{
				IO::debug("Species " + to_string(species_num) + ": " +
					to_string(species.size()) + " individuals\n");
				++species_num;
			}
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_eliminate_stagnating_species()
	{
		typename std::vector<Species<Genotype>> tmp_species;

		for (Species<Genotype>& species : _species)
		{
			if (!species.is_stagnating() && species.size() > 0)
			{
				tmp_species.push_back(species);
			}
		}

		// only delete if not every species is stagnating
		if (tmp_species.size() > 0)
		{
			_species = tmp_species;
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	Genotype_Container<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		_get_champions()
	{
		Genotype_Container<Genotype> elite;

		for (Species<Genotype>& species : _species)
		{
			elite.push_back(species.get_champion());
		}

		return elite;
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	size_t Neat_Evolution_Manager<Genotype, Phenotype>::_calculate_offspring_num(
		size_t t_total_offspring_num, Species<Genotype>& t_species)
	{
		// normal calculation - depends on species fitness share in total fitness
		if (_total_species_fitness > 0)
		{
			float portion = t_species.get_fitness_sq_avg() /
				_total_species_fitness;
			int offspring_num = static_cast<int>(portion * t_total_offspring_num);

			if constexpr(NEAT_EVOLUTION_MANAGER_MATING_DEBUG)
			{
				std::ostringstream tmp_string;

				tmp_string << "Species offspring:\n" <<
					"Percent of total fitness: " << (portion * 100) << "%\n" <<
					"Assigned offspring: " << offspring_num << "/"
					<< t_total_offspring_num << "\n";

				IO::debug(tmp_string.str());
			}

			return offspring_num;
		}
		// should not occur
		else if (_total_species_fitness == 0)
		{
			IO::warning("total species fitness is zero - this means that no "
				"individual has a fitness different from zero\n");
			// not a good way to calculate offspring num - should depend on
			// share of species in propagative population, but would be
			// more complicated to calculate and should not be necessary in the
			// first place
			return t_total_offspring_num / _species.size();
		}
		else
		{
			IO::error("total fitness must not be negative\n");
			return -1;
		}
	}
	
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::
		_produce_species_offspring(Species<Genotype>& t_species,
			size_t t_offspring_num)
	{
		// mutation, intra- and interspecies mating
		for (uint i = 0; i < t_offspring_num; ++i)
		{
			std::shared_ptr<Genotype>& first_parent =
				t_species.get_random_individual();
			Reproduction_Type reproduce_via =
				first_parent->get_reproduction_type();

			if (reproduce_via == Reproduction_Type::interspecies_reproduction
				&& _species.size() > 1)
			{
				_population.push_back(_interspecies_reproduction(first_parent,
					t_species));
			}
			// sexual reproduction is intraspecies - need to have at least one
			// individual in species
			else if (reproduce_via == Reproduction_Type::sexual_reproduction
				&& t_species.size() > 1)
			{
				_population.push_back(_sexual_reproduction(first_parent,
					t_species));
			}
			// asexual reproduction (mutation)
			else
			{
				_population.push_back(_asexual_reproduction(first_parent));
			}
		}
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	std::shared_ptr<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		_interspecies_reproduction(std::shared_ptr<Genotype>& t_first_parent,
			Species<Genotype>& t_first_parent_species)
	{
		// choose random individual from different species
		Species<Genotype>& random_species = r_gen.different_random_item(
			_species, t_first_parent_species);
		std::shared_ptr<Genotype>& second_parent = 
			random_species.get_random_individual();

		Genotype new_offspring = Genotype::create_offspring(
			*t_first_parent, *second_parent);
		// additional mutate
		Genotype mutated_new_offspring = new_offspring.mutate();
		return std::make_shared<Genotype>(std::move(mutated_new_offspring));
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	std::shared_ptr<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		_sexual_reproduction(std::shared_ptr<Genotype>& t_first_parent,
			Species<Genotype>& t_first_parent_species)
	{
		std::shared_ptr<Genotype> second_parent = t_first_parent_species.
			get_different_random_individual(t_first_parent);

		Genotype new_offspring = Genotype::create_offspring(
			*t_first_parent, *second_parent);
		// additional mutate
		Genotype mutated_new_offspring = new_offspring.mutate();
		return std::make_shared<Genotype>(std::move(mutated_new_offspring));
	}

	template<Genotype_Type Genotype, Phenotype_Type Phenotype>
	std::shared_ptr<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		_asexual_reproduction(std::shared_ptr<Genotype>& t_first_parent)
	{
		Genotype new_offspring = t_first_parent->mutate();
		return std::make_shared<Genotype>(std::move(new_offspring));
	}
}

#endif // !VRNTZT_NEAT_EVOLUTION_MANAGER_TPP
