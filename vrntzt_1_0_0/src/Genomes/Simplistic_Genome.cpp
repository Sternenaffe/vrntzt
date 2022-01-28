// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simplistic Genome

#include <vector>
#include <random>
#include <sstream>

#include "lib/utility/include/trivial_typedefs.hpp"
#include "lib/utility/include/various.hpp"
#include "lib/IO/IO.hpp"

#include "include/Genomes/Simplistic_Genome.hpp"

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

	Genome& Genome::operator=(Genome)
	{
		IO::warning("Genome operator=\n");
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

	ushort Simplistic_Genome::_global_neuron_num = 0;

	Simplistic_Genome::Simplistic_Genome(const ushort t_input_num,
		const ushort t_output_num, vector<Genome> t_genomes)
		:
		input_num(t_input_num),
		output_num(t_output_num),
		fixed_neuron_num(t_input_num + t_output_num + bias_num),
		_genomes(t_genomes),
		_fitness(0)
	{
		IO::debug("init of Simplistic_Genome\n");

		// register neurons
		for (int i = 0; i < _genomes.size(); ++i)
		{
			_add_neuron(_genomes[i].connection.source_neuron);
			_add_neuron(_genomes[i].connection.target_neuron);
		}
	}

	Simplistic_Genome::~Simplistic_Genome()
	{

	}

	Simplistic_Genome::operator Generic_Genome()
	{
		return Generic_Genome();
	}

	float Simplistic_Genome::get_fitness() const
	{
		return _fitness;
	}

	void Simplistic_Genome::set_fitness(float t_fitness)
	{
		_fitness = t_fitness;
	}

	void Simplistic_Genome::add_genome(Genome t_genome)
	{
		_genomes.push_back(t_genome);
		_add_neuron(t_genome.connection.source_neuron);
		_add_neuron(t_genome.connection.target_neuron);
	}

	Simplistic_Genome Simplistic_Genome::create_offspring(Simplistic_Genome& t_other)
	{
		return Simplistic_Genome();
	}

	// add neuron to internal list of connected neurons
	void Simplistic_Genome::_add_neuron(ushort t_neuron)
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

	void Simplistic_Genome::mutate()
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

	ushort Simplistic_Genome::get_neuron_num() const
	{
		return _global_neuron_num;
	}

	ushort Simplistic_Genome::get_hidden_neuron_num() const
	{
		return get_neuron_num() - input_num - output_num - bias_num;
	}

	const vector<Genome>& Simplistic_Genome::get_genome_list() const
	{
		return _genomes;
	}

	void Simplistic_Genome::_mutate_weight(Genome& t_genome)
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

	void Simplistic_Genome::_mutate_new_conn()
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
			temp_string << "Simplistic_Genome: Mutate New Connection" <<
				"\nFrom: " << source_neuron <<
				"\nTo: " << target_neuron << "\n";
			IO::debug(temp_string.str());
		}
	}

	void Simplistic_Genome::_mutate_new_neuron()
	{
		// pick random genome
		size_t t_genome_index = r_gen.randint(_genomes.size() - 1);

		// split connection into 2 + neuron
		Connection& chosen_connection = _genomes[t_genome_index].connection;
		// add neuron
		ushort new_neuron = _global_neuron_num;
		++_global_neuron_num;
		_add_neuron(new_neuron);

		if constexpr (MUTATION_DEBUG)
		{
			std::ostringstream temp_string;
			temp_string << "Simplistic_Genome: Mutate New Neuron" <<
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
}
