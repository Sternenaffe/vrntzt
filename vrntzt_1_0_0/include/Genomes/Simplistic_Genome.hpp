// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT Header
// Simplistic Genome

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * Simplistic Genome Header File
  *
  * most basic genome - used for debugging purposes
  * 
  * Simplistic_Genome holds a global neuron counter, the number of neurons
  * created in all networks. Additionally, there's a _connected_hidden_neurons
  * list which contains all neurons which are used in that specific network.
  * This is required because mutation should only create connections to
  * already included neurons, everything else would be effectless.
  */

// TODO: refactor mutate_new_connection
// TODO: add disable property of Genome
// TODO: check if mutation occures multiple times

// IDEA: should outsource _mutate_weight into Genome class?

// IDEA: chance of new connection/new neuron proportinal to genome num?
// IDEA: let mutation chances be determined by genomes to?

// TODO: for simple_genome find good container for sorted genome
//       (small overhead, append with max O(log n), delete with
//		 max O(log n) (could also be insertion with O(log n), fast looping
//       possible, don't need to be directly indexable)

// IDEA: for feedforward: give every genome a min-distance-from-input
//		 and a max-distance-from-input (intervall)

#ifndef SIMPLE_GENOME_HPP
#define SIMPLE_GENOME_HPP

#include "include/global.hpp"

#include "include/Genomes/IGenome.hpp"
#include "include/Generic_Genome.hpp"

#include "include/Helper/simple_helper.hpp"

namespace vrntzt::neat
{
	constexpr bool MUTATION_DEBUG = false;

	struct Genome final
	{
		// weight transform setting
		//const float 

	private:
		static uint _global_innovation_num;

	public:
		// create Genome with random weighted connection
		explicit Genome(ushort t_source_neuron, ushort t_target_neuron);
		// create Genome with pre-defined connetion weight
		explicit Genome(ushort t_source_neuron, ushort t_target_neuron,
			float t_weight);

		Genome(const Genome& t_other);
		Genome& operator=(Genome);

		// mutates connection weight
		void mutate_weight();
		// randomizes weight
		void randomize_weight();

		// innovation number, unique to this source-target
		// neuron combination
		const uint innovation_num = 0;
		Connection connection;
	};

	class Simplistic_Genome : public IGenome<Simplistic_Genome>
	{
		// chance: 1% = 1000 (1:100_000)
		const int max_chance = 100 * 1000;
		const int weight_mutation_chance = 75 * 1000;
		const int weight_perturb_chance = 90 * 1000;
		// probability that connection will get completely new weight
		const int randomize_weight_chance = 10 * 1000;

		const int add_neuron_chance = 30 * 1000;
		const int add_connection_chance = 50 * 1000;

	public:
		explicit Simplistic_Genome(const ushort t_input_num,
			const ushort t_output_num, std::vector<Genome> t_genomes);
		virtual ~Simplistic_Genome();

		explicit operator Generic_Genome();

		// neuron number of inputs = input_index
		const ushort input_num = 0;
		// neuron number of outputs = output_index + _input_num
		const ushort output_num = 0;
		const ushort bias_num = 1;
		const ushort fixed_neuron_num;

		float get_fitness() const;
		void set_fitness(float t_fitness);

		// add new genome (not default mutation!)
		void add_genome(Genome t_genome);

		Simplistic_Genome create_offspring(Simplistic_Genome& t_other);
		// mutate without crossover
		void mutate(); 

		ushort get_neuron_num() const;
		ushort get_hidden_neuron_num() const;
		// returns read-only Genome list
		const std::vector<Genome>& get_genome_list() const;

	private:
		float _fitness;

		std::vector<Genome> _genomes;
		// holds every hidden neuron which is included in network
		// not sorted
		std::vector<ushort> _connected_hidden_neurons;

		// total global neuron num
		static ushort _global_neuron_num;

		// add neuron - checks if already in _connected_hidden_neurons
		void _add_neuron(ushort t_neuron);

		void _mutate_weight(Genome& t_genome);
		// mutation that adds new random connection
		void _mutate_new_conn();
		// mutation that adds new neuron by spliting existing connection
		void _mutate_new_neuron();
	};
}


#endif // !SIMPLE_GENOME_HPP
