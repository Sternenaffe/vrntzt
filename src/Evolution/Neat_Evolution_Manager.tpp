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

#include "include/Evolution/Neat_Evolution_Manager.hpp"

#include <vector>
#include <string>
#include <algorithm>
// #include <cmath>
#include <numeric>

#include "lib/IO/IO.hpp"
#include <lib/utility/include/trivial_typedefs.hpp>

#include "include/vrntzt_global.hpp"
#include "include/Genomes/Simplistic_Genotype.hpp"

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
		_num_old_species = 0;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Neat_Evolution_Manager<Genotype, Phenotype>::~Neat_Evolution_Manager()
	{
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Genotype_Container<Genotype>& Neat_Evolution_Manager<Genotype, Phenotype>::
		get_population()
	{
		return _population;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::
		create_random_population()
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("creating random population\n");
		}
		
		for (int i = 0; i < _population_size; ++i)
		{
			std::unique_ptr<Genotype> tmp = std::make_unique<Genotype>(
				_input_num, _output_num);
			_population.push_back(std::move(tmp));
		}
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::evolve_population()
	{
		using std::vector;

		Genotype_Container<Genotype> new_population;

		_speciate_population();
		_calculate_species_sq_avg_fitness();

		auto species_iter = _species.begin();
		auto species_fitness_iter = _species_sq_avg_fitness.begin();

		// loop through species and produce offspring
		while (species_iter != _species.end() &&
			species_fitness_iter != _species_sq_avg_fitness.end())
		{
			// calculate number of offsprings based on species fitness
			// formular from NEAT paper
			// need to round in order to get full population size
			int offspring_num = 0;
			// every species (and individual for that matter) have a fitness of zero
			// -> split evenly
			if (_total_species_fitness == 0)
			{
				offspring_num = static_cast<int>(std::round(_population_size /
					_species.size()));
			}
			else
			{
				offspring_num = static_cast<int>(std::round(*species_fitness_iter /
					_total_species_fitness * _population_size));
			}
			
			// champions will be inserted separately
			--offspring_num;

			Genotype_Container<Genotype> species_offspring =
				_produce_species_offspring(*species_iter, offspring_num);

			// append offsprings to population
			for (auto& offspring : species_offspring)
			{
				new_population.push_back(std::move(offspring));
			}

			++species_iter;
			++species_fitness_iter;
		}

		_reset_population();
		// population will be empty except for champions: append new offspring
		// append offsprings to population
		for (auto& offspring : new_population)
		{
			_population.push_back(std::move(offspring));
		}
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_speciate_population()
	{
		if constexpr (NEAT_EVOLUTION_MANAGER_DEBUG)
		{
			IO::debug("speciate population\n");
		}

		using std::vector;

		typename Genotype_Container<Genotype>::iterator iter = _population.begin();

		// take first individuum as reference and place in new species
		// if this is first generation
		// else: existing species will be continue to exist
		if (_species.size() == 0)
		{
			Genotype* first_genotype = iter->get();
			vector<Genotype*> new_species = { first_genotype };
			_species.push_back(new_species);
			++iter;
		}
		// first portion of Genotypes in population are champions from the previous
		// generation and therefor already in a species - don't need to insert them
		// again!
		else
		{
			iter += _num_old_species;
		}

		// loop through whole population and create new species if 
		// the compatibility threshold is exceeded
		for (; iter < _population.end(); ++iter)
		{
			bool make_new_species = true;
			Genotype* current_genotype = iter->get();

			// loop through species and see if distance matches
			for (vector<Genotype*>& species : _species)
			{
				Genotype* species_ref = *species.begin();

				if (species_ref->distance(*current_genotype) < _compatibility_threshold)
				{
					species.push_back(current_genotype);
					make_new_species = false;
					break;
				}
			}

			// no matching species: create new
			if (make_new_species)
			{
				vector<Genotype*> new_species = { current_genotype };
				_species.push_back(new_species);
			}
		}

		if constexpr (NEAT_EVOLUTION_MANAGER_SPECIATION_DEBUG)
		{
			using std::to_string;

			IO::debug("\nSpecies: \n");

			size_t species_num = 0;
			for (auto& species : _species)
			{
				IO::debug("Species " + to_string(species_num) + ": " +
					to_string(species.size()) + " individuals\n");
				++species_num;
			}
		}
	}
	
	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Genotype_Container<Genotype> Neat_Evolution_Manager<Genotype, Phenotype>::
		_produce_species_offspring(std::vector<Genotype*>& t_species,
			int t_offspring_num)
	{
		Genotype_Container<Genotype> offspring;

		_eliminate_weak_individuals(t_species);

		// create rest (via intra- and interspecies mating)
		// starts at 1 cause champions is first offspring
		for (int i = 0; i < t_offspring_num; ++i)
		{
			// check if interspecies mating
			if (r_gen.rand() < _interspecies_mating_chance)
			{
				Genotype* first_parent = r_gen.random_item(t_species);
				// choose other random species without check if is same
				std::vector<Genotype*>& random_species = r_gen.random_item(_species);
				Genotype* second_parent = r_gen.random_item(random_species);

				Genotype new_offspring = Genotype::create_offspring(*first_parent,
					*second_parent);
				std::unique_ptr<Genotype> new_offspring_ptr =
					std::make_unique<Genotype>(new_offspring);
				offspring.push_back(std::move(new_offspring_ptr));
			}

			// normal reproduction
			else if (t_species.size() > 1)
			{
				using std::get;

				std::tuple<Genotype*, Genotype*> parents =
					r_gen.random_item_pair(t_species);

				// sexual reproduction
				if (get<0>(parents)->perform_sexual_reproduction())
				{
					Genotype new_offspring = Genotype::create_offspring(
						*get<0>(parents), *get<1>(parents));
					std::unique_ptr<Genotype> new_offspring_ptr =
						std::make_unique<Genotype>(new_offspring);
					offspring.push_back(std::move(new_offspring_ptr));

				}
				// asexual reproduction
				else
				{
					Genotype new_offspring = get<0>(parents)->mutate();
					std::unique_ptr<Genotype> new_offspring_ptr =
						std::make_unique<Genotype>(new_offspring);
					offspring.push_back(std::move(new_offspring_ptr));
				}
			}

			// reproduction if not enough individuals for intraspecies mating
			// -> asexual reproduction
			else
			{
				Genotype* parent = r_gen.random_item(t_species);
				Genotype new_offspring = parent->mutate();
				std::unique_ptr<Genotype> new_offspring_ptr =
					std::make_unique<Genotype>(new_offspring);
				offspring.push_back(std::move(new_offspring_ptr));
			}
		}

		return offspring;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::_reset_population()
	{
		// need to check if is stagnant!
		// maybe check if fitness of old champion is smaller?

		using std::vector;

		// store champions when population is reseted
		Genotype_Container<Genotype> tmp_champion_storage;

		// loop through all species & move champion pointers
		for (vector<Genotype*>& species : _species)
		{
			Genotype* old_champion = _find_champion(species);
			std::unique_ptr<Genotype> champion =
				std::make_unique<Genotype>(*old_champion);
			tmp_champion_storage.push_back(std::move(champion));
		}

		// reset all population related informations
		_population.clear();
		_num_old_species = _species.size();
		_species.clear();

		// old champions are new population
		// IMPORTANT: RESET_POPULATION AND SPECIATE_POPULATION
		//			  RELY THAT CHAMPIONS ARE AT FRONT OF
		// POPULATION! NEED TO CHANGE FUNCTION IF THAT'S NOT GUARANTEED ANYMORE!
		_population = std::move(tmp_champion_storage);
		// every champion is in own species
		for (std::unique_ptr<Genotype>& champion : _population)
		{
			vector<Genotype*> new_species = { champion.get() };
			_species.push_back(std::move(new_species));
		}
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::
		_calculate_species_sq_avg_fitness()
	{
		using std::vector;

		// lambda for adding fitness
		auto add_fitness = [](const float& t_total,
			const Genotype* t_next)
		{
			float fitness = t_next->get_fitness();

			if (fitness < 0)
			{
				IO::error("error: genotypes has negative fitness\n");
			}

			return t_total + fitness;
		};

		_species_sq_avg_fitness.clear();
		_total_species_fitness = 0;

		for (vector<Genotype*>& species : _species)
		{
			double fitness_sum = std::accumulate(species.begin(), species.end(),
				0.0f, add_fitness);
			float sq_fitness = static_cast<float>(fitness_sum /
				pow(species.size(), 2));

			_species_sq_avg_fitness.push_back(sq_fitness);
			_total_species_fitness += sq_fitness;
		}

		if constexpr (NEAT_EVOLUTION_MANAGER_EVOLUTION_DEBUG)
		{
			using std::to_string;

			IO::debug("\nSpecies square average: \n");
			IO::debug("Total fitness sum: " + to_string(_total_species_fitness) + "\n");

			for (size_t i = 0; i < _species.size(); ++i)
			{
				IO::debug("Species " + to_string(i) + " fitness: " +
					to_string(_species_sq_avg_fitness[i]) + "\n");
			}
		}
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	Genotype* Neat_Evolution_Manager<Genotype, Phenotype>::
		_find_champion(std::vector<Genotype*>& t_genotypes)
	{
		typename std::vector<Genotype*>::iterator champion_iter =
			t_genotypes.begin();

		// find champion genotype
		for (typename std::vector<Genotype*>::iterator iter = t_genotypes.begin();
			iter != t_genotypes.end(); ++iter)
		{
			// check if new champion
			if ((*iter)->get_fitness() > (*champion_iter)->get_fitness())
			{
				champion_iter = iter;
			}
		}

		return *champion_iter;
	}

	template <Genotype_Type Genotype, Phenotype_Type Phenotype>
	void Neat_Evolution_Manager<Genotype, Phenotype>::
		_eliminate_weak_individuals(std::vector<Genotype*>& t_species)
	{
		// comparison lambda: returns true if first arg is smaller
		auto comp_genotypes = [](const Genotype* genotype1,
			const Genotype* genotype2)
		{
			return genotype1->get_fitness() < genotype2->get_fitness();
		};

		std::sort(t_species.begin(), t_species.end(), comp_genotypes);

		// remove first '(1 - PROPAGABLE_FRACTION) * total' genotypes
		// which have lowest fitness
		size_t remove_num = static_cast<int>((1 - PROPAGABLE_FRACTION) *
			t_species.size());
		t_species.erase(t_species.begin(), t_species.begin() + remove_num);
	}
}

#endif // !VRNTZT_NEAT_EVOLUTION_MANAGER_TPP
