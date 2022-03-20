// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simplistic Genome

#include "include/vrntzt_pch.h"

#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cmath>

#include "lib/utility/include/trivial_typedefs.hpp"
#include "lib/utility/include/various.hpp"
#include "lib/IO/IO.hpp"

#include "include/vrntzt_global.hpp"
#include "include/Genotype/Simplistic_Genotype.hpp"

namespace vrntzt::neat
{
	using std::vector;
	using Const_Genome_Iterator = Simplistic_Genotype::Const_Genome_Iterator;
	using Iterator_State = Simplistic_Genotype::Const_Genome_Iterator::Iterator_State;

	uint Genome::_global_innovation_num = 0;

	Genome::Genome()
		:
		connection(Connection(0, 0, 0))
	{

	}

	Genome::Genome(size_t t_source_neuron, size_t t_target_neuron)
		:
		innovation_num(++_global_innovation_num),
		connection(Connection(
			t_source_neuron,
			t_target_neuron,
			// value taken from original NEAT implementation
			r_gen.rand(-1, 1)))
	{
		
	}

	Genome::Genome(size_t t_source_neuron, size_t t_target_neuron,
		float t_weight)
		:
		innovation_num(++_global_innovation_num),
		connection(Connection(t_source_neuron, t_target_neuron,
			t_weight))
	{

	}

	Genome::Genome(const Genome& t_other)
		:
		innovation_num(t_other.innovation_num),
		connection(t_other.connection)
	{
	}

	Genome& Genome::operator=(Genome t_other)
	{
		using std::swap;
		innovation_num = t_other.innovation_num;
		connection = t_other.connection;
		return *this;
	}

	/*Genome& Genome::operator=(Genome t_other)
	{
		swap(*this, t_other);
		return *this;
	}*/

	void Genome::mutate_weight()
	{
		// value taken from original NEAT implementation
		connection.weight += r_gen.rand(-4, 4);
	}

	void Genome::randomize_weight()
	{
		// value taken from original NEAT implementation
		connection.weight = r_gen.rand(-4, 4);
	}

	Genome Genome::crossover(const Genome& t_first, const Genome& t_second)
	{
		if (t_first.innovation_num != t_second.innovation_num)
		{
			IO::error("Genome::crossover: Genomes aren't compatible!\n");
			// arbitrary return to make warning go away
			return t_first;
		}

		else
		{
			// copy genome and alter weight
			Genome new_genome(t_first);
			float avg_weight = (t_first.connection.weight +
				t_second.connection.weight) / 2;
			new_genome.connection.weight = avg_weight;

			return new_genome;
		}
	}

	void Genome::reset_innovation_num()
	{
		_global_innovation_num = 0;
	}

	Genome Genome::load_from_xml(pugi::xml_node t_xml_node)
	{
		Genome tmp_genome;

		if (std::strcmp(t_xml_node.name(), "Genome"))
		{
			IO::error("loading file is not valid\n");
		}

		pugi::xml_attribute innovation_attr = t_xml_node.first_attribute();
		if (std::strcmp(innovation_attr.name(), "Innovation_Number"))
		{
			IO::error("loading file is not valid\n");
		}
		tmp_genome.innovation_num = innovation_attr.as_uint();
		_global_innovation_num = std::max(tmp_genome.innovation_num,
			_global_innovation_num);

		pugi::xml_attribute source_neuron_attr = innovation_attr.next_attribute();
		if (std::strcmp(source_neuron_attr.name(), "Source_Neuron"))
		{
			IO::error("loading file is not valid\n");
		}
		tmp_genome.connection.source_neuron = source_neuron_attr.as_uint();

		pugi::xml_attribute target_neuron_attr = source_neuron_attr.next_attribute();
		if (std::strcmp(target_neuron_attr.name(), "Target_Neuron"))
		{
			IO::error("loading file is not valid\n");
		}
		tmp_genome.connection.target_neuron = target_neuron_attr.as_uint();

		pugi::xml_attribute weight_attr = target_neuron_attr.next_attribute();
		if (std::strcmp(weight_attr.name(), "Weight"))
		{
			IO::error("loading file is not valid\n");
		}
		tmp_genome.connection.weight = weight_attr.as_float();

		return tmp_genome;
	}

	void Genome::save_to_xml(pugi::xml_node t_xml_node)
	{
		// append genome node
		pugi::xml_node genotype_node = t_xml_node.append_child("Genome");
		genotype_node.append_attribute("Innovation_Number").set_value(
			innovation_num);
		genotype_node.append_attribute("Source_Neuron").set_value(
			connection.source_neuron);
		genotype_node.append_attribute("Target_Neuron").set_value(
			connection.target_neuron);
		genotype_node.append_attribute("Weight").set_value(
			connection.weight);
	}

	size_t Simplistic_Genotype::_global_neuron_num = 0;

	Simplistic_Genotype::Simplistic_Genotype(const size_t t_input_num,
		const size_t t_output_num)
		:
		input_num(t_input_num),
		output_num(t_output_num),
		fixed_neuron_num(t_input_num + t_output_num + bias_num),
		_fitness(0)
	{
		if constexpr(SIMPLISTIC_GENOTYPE_INIT_DEBUG)
		{
			IO::debug("init of Simplistic_Genotype without genomes\n"
				"creating one random connection\n");
		}

		_register_neuron(fixed_neuron_num - 1);

		_mutate_new_conn();
	}

	Simplistic_Genotype::Simplistic_Genotype(const size_t t_input_num,
		const size_t t_output_num, vector<Genome> t_genomes)
		:
		input_num(t_input_num),
		output_num(t_output_num),
		fixed_neuron_num(t_input_num + t_output_num + bias_num),
		_fitness(0)
	{
		if constexpr (SIMPLISTIC_GENOTYPE_INIT_DEBUG)
		{
			IO::debug("Init of Simplistic_Genotype\n");
		}

		_register_neuron(fixed_neuron_num - 1);

		// add genomes
		for (Genome& genome : t_genomes)
		{
			add_genome(genome);
		}
	}

	Simplistic_Genotype::~Simplistic_Genotype()
	{

	}

	Simplistic_Genotype::Simplistic_Genotype(
		const Simplistic_Genotype& t_other)
		:
		input_num(t_other.input_num),
		output_num(t_other.output_num),
		fixed_neuron_num(t_other.fixed_neuron_num),
		_genomes(t_other._genomes),
		_connected_hidden_neurons(t_other._connected_hidden_neurons),
		_fitness(0),
		_interspecies_reproduction_chance(
			t_other._interspecies_reproduction_chance),
		_sexual_reproduction_chance(
			t_other._sexual_reproduction_chance),
		_no_match_genome_chance(
			t_other._no_match_genome_chance),
		_weight_mutation_chance(
			t_other._weight_mutation_chance),
		_weight_perturb_chance(
			t_other._weight_perturb_chance),
		_randomize_weight_chance(
			t_other._randomize_weight_chance),
		_add_neuron_chance(
			t_other._add_neuron_chance),
		_add_connection_chance(
			t_other._add_connection_chance),
		_delete_connection_chance(
			t_other._delete_connection_chance)
	{
	}

	Simplistic_Genotype& Simplistic_Genotype::operator=(
		Simplistic_Genotype t_other)
	{
		swap(*this, t_other);
		return *this;
	}

	Simplistic_Genotype::operator Generic_Genome()
	{
		return Generic_Genome();
	}

	void Simplistic_Genotype::_delete_genomes()
	{
		_genomes.clear();
	}

	std::string Simplistic_Genotype::get_genomes_string() const
	{
		std::ostringstream tmp_string;
		
		if (_genomes.size() == 0)
		{
			return "None";
		}

		for (const Genome& genome : _genomes)
		{
			tmp_string << genome.innovation_num << " ";
		}

		return tmp_string.str();
	}

	float Simplistic_Genotype::get_fitness() const
	{
		return _fitness;
	}

	void Simplistic_Genotype::set_fitness(float t_fitness)
	{
		if (t_fitness < 0)
		{
			IO::error("fitness must be non-negative value\n");
		}

		_fitness = t_fitness;
	}

	void Simplistic_Genotype::add_genome(Genome t_genome)
	{
		// innovation num bigger: just add to end of list
		if (_genomes.size() == 0 ||
			t_genome.innovation_num > _genomes.back().innovation_num)
		{
			_genomes.push_back(t_genome);
		}
		else
		{
			for (auto it = _genomes.begin(); it < _genomes.end(); ++it)
			{
				if (it->innovation_num == t_genome.innovation_num)
				{
					IO::error("Error: duplicate genome!\n");
					break;
				}
				// first genome with higher innovation num: add before
				else if (it->innovation_num > t_genome.innovation_num)
				{
					_genomes.insert(it, t_genome);
					break;
				}
			}
		}
		
		_register_neuron(t_genome.connection.source_neuron);
		_register_neuron(t_genome.connection.target_neuron);
	}

	float Simplistic_Genotype::distance(const Simplistic_Genotype& t_other)
	{
		// counts number of disjoint and excess genomes and computes
		// average weight difference of matching genome
		// this stats are used for distance computation

		float avg_weight_diff = 0.0f;
		int num_matching_genomes = 0;
		int num_disjoint_genomes = 0;
		int num_excess_genomes = 0;

		Const_Genome_Iterator genome_iter(_genomes, t_other._genomes);

		while (!genome_iter.end())
		{
			// matching genome
			if (genome_iter.get_iterator_state() ==
				Iterator_State::both_lists_equal)
			{
				++num_matching_genomes;
				avg_weight_diff += std::abs(
					genome_iter.first_genome().connection.weight -
					genome_iter.second_genome().connection.weight
				);
			}
			// excess genome
			else if (genome_iter.first_end_reached() ||
				genome_iter.second_end_reached())
			{
				++num_excess_genomes;
			}
			// disjoint genome
			else
			{
				++num_disjoint_genomes;
			}

			genome_iter.advance();
		}

		if (num_matching_genomes > 0)
		{
			avg_weight_diff /= num_matching_genomes;
		}
		// normalize number of excess and disjoint genomes
		size_t total_genome_num = std::max(_genomes.size(),
			t_other._genomes.size());
		float portion_disjoint_genomes = 1.0f * num_disjoint_genomes / 
			total_genome_num;
		float portion_excess_genomes = 1.0f * num_excess_genomes /
			total_genome_num;

		// calculate final distance
		float distance = _distance_disjoint_relevance * portion_disjoint_genomes +
			_distance_excess_relevance * portion_excess_genomes +
			_distance_weight_relevance * avg_weight_diff;

		if constexpr (SIMPLISTIC_GENOTYPE_DISTANCE_DEBUG)
		{
			std::ostringstream tmp_string;

			tmp_string << "Distance:\n" <<
				"Parent 1 Genomes: " << get_genomes_string() << "\n" <<
				"Parent 2 Genomes: " << t_other.get_genomes_string() << "\n" <<
				"distance: " << distance << "\n";

			IO::debug(tmp_string.str());
		}

		return distance;
	}

	Reproduction_Type Simplistic_Genotype::get_reproduction_type()
	{
		if (r_gen.rand() <= _interspecies_reproduction_chance)
		{
			return Reproduction_Type::interspecies_reproduction;
		}
		else if (r_gen.rand() <= _sexual_reproduction_chance)
		{
			return Reproduction_Type::sexual_reproduction;
		}
		else
		{
			return Reproduction_Type::asexual_reproduction;
		}
	}

	Simplistic_Genotype Simplistic_Genotype::create_offspring(
		const Simplistic_Genotype& t_first,
		const Simplistic_Genotype& t_second)
	{
		// requires _genomes to be sorted by innovation num
		if (t_first.get_fitness() > t_second.get_fitness())
		{
			return t_first._mate(t_second, false);
		}

		if (t_first.get_fitness() < t_second.get_fitness())
		{
			return t_second._mate(t_first, false);
		}

		// same fitness; rare edge case
		else
		{
			return t_first._mate(t_second, true);
		}	
	}

	// add neuron to internal list of connected neurons
	void Simplistic_Genotype::_register_neuron(size_t t_neuron)
	{
		// increase _global_neuron_num if unknown; equivalent to adding 
		// new neuron
		if (t_neuron >= _global_neuron_num)
		{
			_global_neuron_num = t_neuron + 1;
		}

		// don't add to hidden neuron list if is input, output or bias
		if (t_neuron < input_num + output_num + bias_num)
		{
			return;
		}

		auto item_pos = std::find(_connected_hidden_neurons.begin(),
			_connected_hidden_neurons.end(), t_neuron);
		bool already_included = item_pos != _connected_hidden_neurons.end();
		if (!already_included)
		{
			_connected_hidden_neurons.push_back(t_neuron);
		}
	}

	const std::vector<size_t>& Simplistic_Genotype::get_hidden_neurons() const
	{
		return _connected_hidden_neurons;
	}

	void Simplistic_Genotype::reset_global_neuron_num()
	{
		_global_neuron_num = 0;
	}

	// if same fitness = false: t_other is genome with lower fitness
	Simplistic_Genotype Simplistic_Genotype::_mate(
		const Simplistic_Genotype& t_other, const bool t_same_fitness)
		const
	{
		vector<Genome> offspring_genomes;

		// if different fitness: first genome list needs to be of genotype
		// with higher fitness!
		const vector<Genome>& higher_fitness_genomes =
			get_fitness() > t_other.get_fitness() ? _genomes : t_other._genomes;
		const vector<Genome>& lower_fitness_genomes =
			get_fitness() < t_other.get_fitness() ? t_other._genomes : _genomes;
		
		Const_Genome_Iterator genomes_iter(higher_fitness_genomes,
			lower_fitness_genomes);

		while (!genomes_iter.end())
		{	
			if (t_same_fitness)
			{
				_next_same_fitness_mating_step(offspring_genomes,
					genomes_iter);
			}
			else
			{
				_next_diff_fitness_mating_step(offspring_genomes,
					genomes_iter);
			}

			genomes_iter.advance();
		}

		Simplistic_Genotype offspring(input_num, output_num,
			offspring_genomes);

		offspring._calculate_reproduction_settings(*this, t_other);

		if constexpr (SIMPLISTIC_GENOTYPE_MATING_DEBUG)
		{
			std::ostringstream tmp_string;

			tmp_string << "Mating:\n" <<
				"Parent 1 Genomes: " << get_genomes_string() << "\n" <<
				"Parent 1 Fitness: " << get_fitness() << "\n" <<
				"Parent 2 Genomes: " << t_other.get_genomes_string() << "\n" <<
				"Parent 2 Fitness: " << t_other.get_fitness() << "\n" <<
				"Offspring Genomes: " << offspring.get_genomes_string() << "\n";

			IO::debug(tmp_string.str());
		}

		// don't allow offspring to have no genomes - might happen
		// when same fitness and no matching genomes
		if (offspring._genomes.size() == 0)
		{
			// new Genotype with random connection
			return Simplistic_Genotype(input_num, output_num);
		}

		return offspring;
	}

	void Simplistic_Genotype::_calculate_reproduction_settings(const Simplistic_Genotype& t_first, const Simplistic_Genotype& t_second)
	{
		if (ENABLE_REPRODUCTION_SETTING_MUTATION)
		{
			_interspecies_reproduction_chance =
				(t_first._interspecies_reproduction_chance +
				 t_second._interspecies_reproduction_chance) / 2;
			_sexual_reproduction_chance =
				(t_first._sexual_reproduction_chance +
				 t_second._sexual_reproduction_chance) / 2;
			_no_match_genome_chance =
				(t_first._no_match_genome_chance +
				 t_second._no_match_genome_chance) / 2;
			_weight_mutation_chance =
				(t_first._weight_mutation_chance +
				 t_second._weight_mutation_chance) / 2;
			_weight_perturb_chance = 
				(t_first._weight_perturb_chance +
				 t_second._weight_perturb_chance) / 2;
			_randomize_weight_chance = 
				(t_first._randomize_weight_chance +
				 t_second._randomize_weight_chance) / 2;
			_add_neuron_chance = 
				(t_first._add_neuron_chance +
				 t_second._add_neuron_chance) / 2;
			_add_connection_chance =
				(t_first._add_connection_chance +
				 t_second._add_connection_chance) / 2;
			_delete_connection_chance =
				(t_first._delete_connection_chance +
				 t_second._delete_connection_chance) / 2;
		}
	}

	void Simplistic_Genotype::_next_same_fitness_mating_step(
		std::vector<Genome>& t_genomes,
		const Const_Genome_Iterator& t_iter)
		const
	{
		switch (t_iter.get_iterator_state())
		{
		// matching genomes: average over weights
		case Iterator_State::both_lists_equal:
		{
			Genome new_genome = Genome::crossover(t_iter.first_genome(),
				t_iter.second_genome());
			t_genomes.push_back(std::move(new_genome));
			break;
		}
		// not matching genomes: add genome with certain probability
		case Iterator_State::first_genome_list:
		{
			if (_chance_add_genome())
			{
				t_genomes.push_back(t_iter.first_genome());
			}
			break;
		}
		case Iterator_State::second_genome_list:
		{
			if (_chance_add_genome())
			{
				t_genomes.push_back(t_iter.second_genome());
			}
			break;
		}
		}
	}

	void Simplistic_Genotype::_next_diff_fitness_mating_step(
		std::vector<Genome>& t_genomes,
		const Const_Genome_Iterator& t_iter)
		const
	{
		switch (t_iter.get_iterator_state())
		{
		// matching genomes: average over weights
		case Iterator_State::both_lists_equal:
		{
			Genome new_genome = Genome::crossover(t_iter.first_genome(),
				t_iter.second_genome());
			t_genomes.push_back(std::move(new_genome));
			break;
		}
		// not matching genomes: add genome with certain probability
		// first_genome_list will be of genotype with higher fitness!
		case Iterator_State::first_genome_list:
		{
			t_genomes.push_back(t_iter.first_genome());
			break;
		}
		// lower fitness genotype: do nothing
		case Iterator_State::second_genome_list:
		{
			break;
		}
		}
	}

	bool Simplistic_Genotype::_chance_add_genome() const
	{
		return r_gen.rand() <= _no_match_genome_chance;
	}

	Simplistic_Genotype Simplistic_Genotype::mutate()
	{
		// if set to true: only one weight mutation occures per generation
		constexpr bool single_weight_mutation = true;

		Simplistic_Genotype new_genotype(*this);
		new_genotype._mutate_reproduction_settings();

		if constexpr (!single_weight_mutation)
		{
			// mutate genome weights
			for (auto& genome : new_genotype._genomes)
			{
				if (r_gen.rand() <= _weight_mutation_chance)
				{
					new_genotype._mutate_weight(genome);
				}
			}
		}
		else
		{
			if (r_gen.rand() <= _weight_mutation_chance)
			{
				// pick random genome & mutate weight
				Genome& r_genome = r_gen.random_item(new_genotype._genomes);
				new_genotype._mutate_weight(r_genome);
			}
		}

		// add connection
		if (r_gen.rand() <= _add_connection_chance)
		{
			new_genotype._mutate_new_conn();
		}

		// add neuron
		if (r_gen.rand() <= _add_neuron_chance)
		{
			new_genotype._mutate_new_neuron();
		}

		// delete connection
		if (r_gen.rand() <= _delete_connection_chance)
		{
			new_genotype._delete_conn();
		}

		return new_genotype;
	}

	size_t Simplistic_Genotype::get_neuron_num() const
	{
		return static_cast<size_t>(fixed_neuron_num +
			_connected_hidden_neurons.size());
	}

	size_t Simplistic_Genotype::get_hidden_neuron_num() const
	{
		return get_neuron_num() - input_num - output_num - bias_num;
	}

	bool Simplistic_Genotype::perform_sexual_reproduction() const
	{
		return r_gen.rand() <= _sexual_reproduction_chance;
	}

	const vector<Genome>& Simplistic_Genotype::get_genome_list() const
	{
		return _genomes;
	}

	// REFACTOR!!!
	void Simplistic_Genotype::_mutate_weight(Genome& t_genome)
	{
		if (r_gen.rand() <= _randomize_weight_chance)
		{
			t_genome.randomize_weight();
		}
		else
		{
			t_genome.mutate_weight();
		}
	}

	void Simplistic_Genotype::_mutate_new_conn()
	{
		// source neuron: could be every connected neuron except outputs
		// numbers < input number represent index of input neuron
		size_t source_neuron = 0;
		// -1 cause bounds are included
		size_t r_num = r_gen.randint(bias_num + input_num +
			_connected_hidden_neurons.size() - 1);
		// source is input neuron
		if (r_num < input_num + bias_num)
		{
			source_neuron = r_num;
		}
		// source is hidden neuron - pick from list of hidden neurons
		else
		{
			size_t index = r_num - input_num - bias_num;
			source_neuron = _connected_hidden_neurons[index];
		}

		// target neuron: could be every connected neuron except
		// input & bias
		size_t target_neuron = 0;
		// create number between (input_num + bias_num) and
		// total neuron num
		// -1 cause bounds are included
		r_num = r_gen.randint(static_cast<int>(input_num + bias_num),
			static_cast<int>(fixed_neuron_num + _connected_hidden_neurons.size() - 1));
		// target is output
		if (r_num < fixed_neuron_num)
		{
			target_neuron = r_num;
		}
		// target is hidden neuron
		else
		{
			target_neuron = _connected_hidden_neurons[
				r_num - fixed_neuron_num];
		}

		// add genome with random weight
		_genomes.push_back(Genome(source_neuron, target_neuron));

		if constexpr (SIMPLISTIC_GENOTYPE_MUTATION_DEBUG)
		{
			std::ostringstream temp_string;
			temp_string << "Simplistic_Genotype: Mutate New Connection" <<
				"\nFrom: " << source_neuron <<
				"\nTo: " << target_neuron << "\n";
			IO::debug(temp_string.str());
		}
	}

	void Simplistic_Genotype::_mutate_new_neuron()
	{
		if (_genomes.size() == 0)
		{
			return;
		}

		// pick random genome
		size_t t_genome_index = r_gen.randint(static_cast<int>(_genomes.size() - 1));

		// split connection into 2 + neuron
		Connection chosen_connection = _genomes[t_genome_index].connection;
		_genomes.erase(_genomes.begin() + t_genome_index);

		// add neuron
		size_t new_neuron = _global_neuron_num;
		_register_neuron(new_neuron);

		if constexpr (SIMPLISTIC_GENOTYPE_MUTATION_DEBUG)
		{
			std::ostringstream temp_string;
			temp_string << "Simplistic_Genotype: Mutate New Neuron" <<
				"\nBetween: " << chosen_connection.source_neuron <<
				" and " << chosen_connection.target_neuron <<
				"\nNeuron Number: " << new_neuron << "\n";
			IO::debug(temp_string.str());
		}

		// add connection to new neuron; same source & weight
		// as old connection
		_genomes.push_back(Genome(chosen_connection.source_neuron,
			new_neuron, chosen_connection.weight));
		// replace old connection new one with same target and weight 1
		// replacement not possible cause genomes need to be sorted
		_genomes.push_back(Genome(new_neuron,
			chosen_connection.target_neuron, 1.0f));
	}

	void Simplistic_Genotype::_delete_conn()
	{
		// avoid empty container
		if (_genomes.size() == 1)
		{
			return;
		}

		// pick random genome and remove
		size_t t_genome_index = r_gen.randint(static_cast<int>(_genomes.size() - 1));
		_genomes.erase(_genomes.begin() + t_genome_index);
	}

	void Simplistic_Genotype::_mutate_reproduction_settings()
	{
		if (ENABLE_REPRODUCTION_SETTING_MUTATION)
		{
			_mutate_reproduction_dimension(_interspecies_reproduction_chance);
			_mutate_reproduction_dimension(_sexual_reproduction_chance);
			_mutate_reproduction_dimension(_no_match_genome_chance);
			_mutate_reproduction_dimension(_weight_mutation_chance);
			_mutate_reproduction_dimension(_weight_perturb_chance);
			_mutate_reproduction_dimension(_randomize_weight_chance);
			_mutate_reproduction_dimension(_add_neuron_chance);
			_mutate_reproduction_dimension(_add_connection_chance);
			_mutate_reproduction_dimension(_delete_connection_chance);
		}
	}

	void Simplistic_Genotype::_mutate_reproduction_dimension(float& t_value)
	{
		// not to small if dimension is very small - this avoids very
		// long mutation time if is really wrong
		// 1%	-> change of up to 0.4%
		// 10%	-> change of up to 3.5%
		// 100% -> change of up to 10%
		const float delta = 0.1f * (-pow(t_value - 1, 2) + 1);
		// avoids 0 or lower
		const float lower_bound = t_value - delta;
		const float upper_bound = t_value + delta;

		// with 10%: high mutation (5% high, 5% lower)
		float chance = r_gen.rand();
		if (chance < 0.05)
		{
			t_value = r_gen.rand(upper_bound, upper_bound + delta);
		}
		else if (chance < 0.1)
		{
			t_value = r_gen.rand(lower_bound - delta, lower_bound);
		}
		// normal mutation
		else
		{
			// dimension is random value between lower and upper bound
			t_value = r_gen.rand(lower_bound, upper_bound);
		}

		// value should between 1 and 0.0005
		t_value = std::min(1.0f, std::max(0.0005f, t_value));
	}

	/*void swap(Genome& t_first, Genome& t_second)
	{
		using std::swap;
		t_first.connection = t_second.connection;
	}*/

	void swap(Simplistic_Genotype& t_first, Simplistic_Genotype& t_second)
	{
		using std::swap;

		t_first._interspecies_reproduction_chance =
			t_second._interspecies_reproduction_chance;
		t_first._sexual_reproduction_chance =
			t_second._sexual_reproduction_chance;
		t_first._no_match_genome_chance =
			t_second._no_match_genome_chance;
		t_first._weight_mutation_chance =
			t_second._weight_mutation_chance;
		t_first._weight_perturb_chance =
			t_second._weight_perturb_chance;
		t_first._randomize_weight_chance =
			t_second._randomize_weight_chance;
		t_first._add_neuron_chance =
			t_second._add_neuron_chance;
		t_first._add_connection_chance =
			t_second._add_connection_chance;
		t_first._delete_connection_chance =
			t_second._delete_connection_chance;

		swap(t_first._genomes, t_second._genomes);
		swap(t_first._connected_hidden_neurons,
			t_second._connected_hidden_neurons);
	}

	void Simplistic_Genotype::save_to_xml(pugi::xml_node t_xml_node)
	{
		// append genotype node
		pugi::xml_node genotype_node = t_xml_node.append_child("Simplistic_Genotype");

		// add genomes
		for (auto& genome : _genomes)
		{
			genome.save_to_xml(genotype_node);
		}
	}

	void Simplistic_Genotype::load_from_xml(pugi::xml_node t_genotype_node)
	{
		if (std::strcmp(t_genotype_node.name(), "Simplistic_Genotype"))
		{
			IO::error("loading file is not valid\n");
		}

		_delete_genomes();

		// load genomes
		for (pugi::xml_node genome_node = t_genotype_node.first_child();
			genome_node; genome_node = genome_node.next_sibling())
		{
			Genome tmp_genome = tmp_genome.load_from_xml(genome_node);
			// find biggest used neuron number
			_global_neuron_num = std::max({ tmp_genome.connection.source_neuron,
				tmp_genome.connection.target_neuron, _global_neuron_num });

			add_genome(std::move(tmp_genome));
		}
	}

	Const_Genome_Iterator::Const_Genome_Iterator(
		const std::vector<Genome>& t_first_genomes,
		const std::vector<Genome>& t_second_genomes)
		:
		_first_genomes(t_first_genomes),
		_second_genomes(t_second_genomes),
		_first_iter(t_first_genomes.begin()),
		_second_iter(t_second_genomes.begin())
	{
		_update_iter_state();
	}

	void Const_Genome_Iterator::advance()
	{
		// handle special cases (iterator(s) reached end)
		if (end())
		{
			IO::error("can't advance iterator; already reached end\n");
			return;
		}
		// if end of first vector is reached, but not of second
		else if (first_end_reached())
		{
			++_second_iter;
		}
		// if end of second vector is reached, but not of first
		else if (second_end_reached())
		{
			++_first_iter;
		}

		// standard advance: based on iter state
		// same innovation number
		else if (_iter_state == Iterator_State::both_lists_equal)
		{
			++_first_iter;
			++_second_iter;
		}
		// first genome has lower innovation num
		else if (_iter_state == Iterator_State::first_genome_list)
		{
			++_first_iter;
		}
		// second genome has lower innovation num
		else if (_iter_state == Iterator_State::second_genome_list)
		{
			++_second_iter;
		}
		else
		{
			IO::error("unexpected error!\n");
		}

		_update_iter_state();
	}

	Iterator_State Const_Genome_Iterator::get_iterator_state() const
	{
		return _iter_state;
	}

	const Genome& Const_Genome_Iterator::first_genome() const
	{
		return *_first_iter;
	}

	const Genome& Const_Genome_Iterator::second_genome() const
	{
		return *_second_iter;
	}

	void Const_Genome_Iterator::_update_iter_state()
	{
		// end will be handled by advance() - this is next step so result
		// would be different!
		
		// if end of first vector is reached, but not of second
		if (first_end_reached())
		{
			_iter_state = Iterator_State::second_genome_list;
		}
		// if end of second vector is reached, but not of first
		else if (second_end_reached())
		{
			_iter_state = Iterator_State::first_genome_list;
		}
		// same innovation number
		else if (_first_iter->innovation_num == _second_iter->innovation_num)
		{
			_iter_state = Iterator_State::both_lists_equal;
		}
		// first genome has lower innovation num
		else if (_first_iter->innovation_num < _second_iter->innovation_num)
		{
			_iter_state = Iterator_State::first_genome_list;
		}
		// second genome has lower innovation num
		else if (_first_iter->innovation_num > _second_iter->innovation_num)
		{
			_iter_state = Iterator_State::second_genome_list;
		}
		else
		{
			IO::error("unexpected error!\n");
		}
	}

	bool Const_Genome_Iterator::end() const
	{
		return (_first_iter == _first_genomes.end() &&
			_second_iter == _second_genomes.end());
	}

	bool Const_Genome_Iterator::first_end_reached() const
	{
		return (_first_iter == _first_genomes.end());
	}

	bool Const_Genome_Iterator::second_end_reached() const
	{
		return (_second_iter == _second_genomes.end());
	}
}
