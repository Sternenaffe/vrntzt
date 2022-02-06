// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simplistic Genome

#include <vector>
#include <random>
#include <string>
#include <sstream>

#include "lib/utility/include/trivial_typedefs.hpp"
#include "lib/utility/include/various.hpp"
#include "lib/IO/IO.hpp"

#include "include/Genomes/Simplistic_Genotype.hpp"

namespace vrntzt::neat
{
	using std::vector;

	uint Genome::_global_innovation_num = 0;

	// create random generators
	u_lib::Random_Generator r_gen;

	Genome::Genome(ushort t_source_neuron, ushort t_target_neuron)
		:
		innovation_num(++_global_innovation_num),
		connection(Connection(
			t_source_neuron,
			t_target_neuron,
			// value taken from original NEAT implementation
			r_gen.rand(-1, 1)))
	{
		
	}

	Genome::Genome(ushort t_source_neuron, ushort t_target_neuron,
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
		swap(*this, t_other);
		return *this;
	}

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

	ushort Simplistic_Genotype::_global_neuron_num = 0;

	Simplistic_Genotype::Simplistic_Genotype(const ushort t_input_num,
		const ushort t_output_num)
		:
		input_num(t_input_num),
		output_num(t_output_num),
		fixed_neuron_num(t_input_num + t_output_num + bias_num),
		_fitness(0)
	{
		IO::debug("init of Simplistic_Genotype without genomes\n");
	}

	Simplistic_Genotype::Simplistic_Genotype(const ushort t_input_num,
		const ushort t_output_num, vector<Genome> t_genomes)
		:
		input_num(t_input_num),
		output_num(t_output_num),
		fixed_neuron_num(t_input_num + t_output_num + bias_num),
		_fitness(0)
	{
		IO::debug("Init of Simplistic_Genotype\n");

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
		_fitness(0)
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

	std::string Simplistic_Genotype::get_genomes_string() const
	{
		std::ostringstream tmp_string;

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
	void Simplistic_Genotype::_register_neuron(ushort t_neuron)
	{
		// increase _global_neuron_num if unknown; equivalent to adding 
		// new neuron
		if (t_neuron >= _global_neuron_num)
		{
			_global_neuron_num = t_neuron;
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

	// if same fitness = false: t_other is genome with lower fitness
	Simplistic_Genotype Simplistic_Genotype::_mate(
		const Simplistic_Genotype& t_other, const bool t_same_fitness)
		const
	{
		vector<Genome> offspring_genomes;

		vector<Genome>::const_iterator genome1_iter =
			_genomes.begin();
		vector<Genome>::const_iterator genome2_iter =
			t_other._genomes.begin();

		while (genome1_iter < _genomes.end() &&
			   genome2_iter < t_other._genomes.end())
		{	
			if (t_same_fitness)
			{
				_next_same_fitness_mating_step(offspring_genomes,
					genome1_iter, genome2_iter);
			}
			else
			{
				_next_diff_fitness_mating_step(offspring_genomes,
					genome1_iter, genome2_iter);
			}
		}

		Simplistic_Genotype offspring(input_num, output_num,
			offspring_genomes);

		if constexpr (MATING_DEBUG)
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

		return offspring;
	}

	void Simplistic_Genotype::_next_same_fitness_mating_step(
		std::vector<Genome>& t_genomes,
		std::vector<Genome>::const_iterator& t_genome1_iter,
		std::vector<Genome>::const_iterator& t_genome2_iter)
		const
	{
		// matching genomes: average over weights
		if (t_genome1_iter->innovation_num == t_genome2_iter->innovation_num)
		{
			Genome new_genome = Genome::crossover(*t_genome1_iter,
				*t_genome2_iter);
			t_genomes.push_back(std::move(new_genome));

			++t_genome1_iter;
			++t_genome2_iter;
		}
		// not matching genomes: add genome with certain probability
		else if (t_genome1_iter->innovation_num < t_genome2_iter->innovation_num)
		{
			if (_chance_add_genome())
			{
				t_genomes.push_back(*t_genome1_iter);
			}
			++t_genome1_iter;
		}

		// genome2.innovation_num < genome1.innovation_num
		else
		{
			if (_chance_add_genome())
			{
				t_genomes.push_back(*t_genome2_iter);
			}
			++t_genome2_iter;
		}
	}

	void Simplistic_Genotype::_next_diff_fitness_mating_step(
		std::vector<Genome>& t_genomes,
		std::vector<Genome>::const_iterator& t_genome1_iter,
		std::vector<Genome>::const_iterator& t_genome2_iter)
		const
	{
		// matching genomes: average over weights
		if (t_genome1_iter->innovation_num == t_genome2_iter->innovation_num)
		{
			Genome new_genome = Genome::crossover(*t_genome1_iter,
				*t_genome2_iter);
			t_genomes.push_back(std::move(new_genome));

			++t_genome1_iter;
			++t_genome2_iter;
		}
		// not matching genomes: add genome if from higher-fitness parent
		else if (t_genome1_iter->innovation_num < t_genome2_iter->innovation_num)
		{
			t_genomes.push_back(*t_genome1_iter);
			++t_genome1_iter;
		}

		// genome2.innovation_num < genome1.innovation_num
		// don't add genome
		else
		{
			++t_genome2_iter;
		}
	}

	bool Simplistic_Genotype::_chance_add_genome() const
	{
		int chance = r_gen.randint(1, max_chance);
		return no_match_genome_chance >= chance;
	}

	void Simplistic_Genotype::mutate()
	{
		int chance = 0;

		// mutate genome weights
		for (size_t i = 0; i < _genomes.size(); ++i)
		{
			chance = r_gen.randint(1, max_chance);
			if (weight_mutation_chance >= chance)
			{
				_mutate_weight(_genomes[i]);
			}
		}

		// add connection
		chance = r_gen.randint(1, max_chance);
		if (add_connection_chance >= chance)
		{
			_mutate_new_conn();
		}

		// add neuron
		chance = r_gen.randint(1, max_chance);
		if (add_neuron_chance >= chance)
		{
			_mutate_new_neuron();
		}
	}

	ushort Simplistic_Genotype::get_neuron_num() const
	{
		return _global_neuron_num;
	}

	ushort Simplistic_Genotype::get_hidden_neuron_num() const
	{
		return get_neuron_num() - input_num - output_num - bias_num;
	}

	const vector<Genome>& Simplistic_Genotype::get_genome_list() const
	{
		return _genomes;
	}

	void Simplistic_Genotype::_mutate_weight(Genome& t_genome)
	{
		int chance = r_gen.randint(1, max_chance);

		if (randomize_weight_chance >= chance)
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
		ushort source_neuron = 0;
		// -1 cause bounds are included
		ushort r_num = r_gen.randint(input_num +
			_connected_hidden_neurons.size() - 1);
		// source is input neuron
		if (r_num < input_num)
		{
			source_neuron = r_num;
		}
		// source is hidden neuron
		else
		{
			source_neuron = _connected_hidden_neurons[r_num - input_num];
		}

		// target neuron: could be every connected neuron except
		// input & bias
		ushort target_neuron = 0;
		// create number between (input_num + bias_num) and
		// total neuron num
		// -1 cause bounds are included
		r_num = r_gen.randint(input_num + bias_num,
			fixed_neuron_num + _connected_hidden_neurons.size() - 1);
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

		if constexpr (MUTATION_DEBUG)
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
		size_t t_genome_index = r_gen.randint(_genomes.size() - 1);

		// split connection into 2 + neuron
		Connection& chosen_connection = _genomes[t_genome_index].connection;
		// add neuron
		ushort new_neuron = _global_neuron_num;
		++_global_neuron_num;
		_register_neuron(new_neuron);

		if constexpr (MUTATION_DEBUG)
		{
			std::ostringstream temp_string;
			temp_string << "Simplistic_Genotype: Mutate New Neuron" <<
				"\nBetween: " << chosen_connection.source_neuron <<
				" and : " << chosen_connection.target_neuron <<
				"\nNeuron Number: " << new_neuron << "\n";
			IO::debug(temp_string.str());
		}

		// add connection to new neuron; same source & weight
		// as old connection
		_genomes.push_back(Genome(chosen_connection.source_neuron,
			new_neuron, chosen_connection.weight));
		// replace old connection new one with same target and weight 1
		// replacement avoids costly remove
		_genomes[t_genome_index] = Genome(new_neuron,
			chosen_connection.source_neuron, 1);
	}

	void swap(Genome& t_first, Genome& t_second)
	{
		using std::swap;

		swap(t_first.connection, t_second.connection);
	}

	void swap(Simplistic_Genotype& t_first, Simplistic_Genotype& t_second)
	{
		using std::swap;

		swap(t_first._genomes, t_second._genomes);
		swap(t_first._connected_hidden_neurons,
			t_second._connected_hidden_neurons);
	}
}
