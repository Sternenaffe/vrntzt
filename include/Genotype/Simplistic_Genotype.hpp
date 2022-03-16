// ch 2021-22
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
  * Simplistic_Genotype holds a global neuron counter, the number of neurons
  * created in all networks. Additionally, there's a _connected_hidden_neurons
  * list which contains all neurons which are used in that specific network.
  * This is required because mutation should only create connections to
  * already included neurons, everything else would be effectless.
  */

// https://stackoverflow.com/questions/10699265/how-can-i-efficiently-select-a-standard-library-container-in-c11/10701102#10701102
// https://stackoverflow.com/questions/491668/levels-of-indentation

// SPLIT: Genome - Genotype - Genome Container

// IMPORTANT: implement copy constructor and assignment operator
// IMPORTANT: change loading to static factory-like method without other params than node
// IMPORTANT: move weight altering from Genome to Connection
// IMPORTANT: add check if fitness was set
// IMPORTANT: add check for loaded attributes

// TODO: split class & responsibilities
// TODO: change chances from int to floats
// TODO: change mating function signatures to static member
// TODO: outsource mutation & crossover to helper class OR
//		 OR make Genotype contructible from two parents (better)
// TODO: make constants CAPS
// TODO: refactor mutate_new_connection
// TODO: add disable property of Genome
// TODO: check if mutation occures multiple times
// TODO: encapsulate connection of Genome
// TODO: outsource _mutate_weight into Genome class?
// TODO: error and bound checks (e.g. no genome/connection)
// TODO: make settings static const
// TODO: change fitness to unsigned
// TODO: check if connection already exists when mutating new connection
// TODO: change connected_hidden_neuron vector: internally every neuron should
//		 have subsequent number, connected_hidden_neurons should map to global
//		 neuron numbers

// IDEA: chance of new connection/new neuron proportinal to genome num?
// IDEA: let mutation chances be determined by genomes to?
// IDEA: distance settings could be made variable with two-way distance
//		 computation (only same species if both genotypes agree)
//		 might not work cause own species is advantage
// IDEA: can a PROPAGABLE_FRACTION gene work? probably but only species-wide

// TODO: for Simple_Genotype find good container for sorted genome
//       (small overhead, append with max O(log n), delete with
//		 max O(log n) (could also be insertion with O(log n), fast looping
//       possible, don't need to be directly indexable)
// TODO: create move constructor and assignment operator

// IDEA: for feedforward: give every genome a min-distance-from-input
//		 and a max-distance-from-input (intervall)

#ifndef SIMPLISTIC_GENOTYPE_HPP
#define SIMPLISTIC_GENOTYPE_HPP

#include "lib/pugixml-1.12/src/pugixml.hpp"

#include "include/Genotype/IGenotype.hpp"
#include "include/Generic_Genome.hpp"

#include "src/Helper/simple_helper.hpp"

namespace vrntzt::neat
{
	constexpr bool SIMPLISTIC_GENOTYPE_INIT_DEBUG = false;
	constexpr bool SIMPLISTIC_GENOTYPE_MUTATION_DEBUG = false;
	constexpr bool SIMPLISTIC_GENOTYPE_MATING_DEBUG = false;
	constexpr bool SIMPLISTIC_GENOTYPE_DISTANCE_DEBUG = false;

	enum class Reproduction_Type
	{
		sexual_reproduction,
		// mutation
		asexual_reproduction,
		// another type of sexual reproduction
		interspecies_reproduction
	};

	struct Genome final
	{
		// weight transform setting
		//const float 
		const float init_weight_min = -1;
		const float init_weight_max = 1;
		const float random_weight_min = -4;
		const float random_weight_max = 4;
		const float perturb_weight_min = -2;
		const float perturb_weight_max = 2;

	private:
		// constructor for loading
		explicit Genome();

		static uint _global_innovation_num;

	public:
		// create Genome with random weighted connection
		explicit Genome(size_t t_source_neuron, size_t t_target_neuron);
		// create Genome with pre-defined connetion weight
		explicit Genome(size_t t_source_neuron, size_t t_target_neuron,
			float t_weight);

		Genome(const Genome& t_other);
		Genome& operator=(Genome t_other);

		// loading constructors
		static Genome load_from_xml(pugi::xml_node t_xml_node);
		// save
		void save_to_xml(pugi::xml_node t_xml_node);

		// mutates connection weight
		void mutate_weight();
		// randomizes weight
		void randomize_weight();

		static Genome crossover(const Genome& t_first, const Genome& t_second);

		// innovation number, unique to this source-target
		// neuron combination
		uint innovation_num = 0;
		Connection connection;
	};

	class Simplistic_Genotype : public IGenotype<Simplistic_Genotype>
	{
		// distance settings
		// values taken from neat paper
		const float distance_disjoint_relevance = 3.0f;
		const float distance_excess_relevance = 3.0f;
		const float distance_weight_relevance = 1.0f;

		// chance: 1% = 1000 (1:100_000)
		const uint max_chance = 100 * 1000;

		// determines how often sexual reproduction with individual which
		// is not compatible will occur
		const uint interspecies_reproduction_chance = static_cast<uint>(0.1 * 1000);
		// determines how often sexual and asexual respectively
		// reproduction will occur
		const uint sexual_reproduction_chance = 75 * 1000;

		// chance that excess/disjoint genomes are picked when
		// parent fitness is same
		const uint no_match_genome_chance = 50 * 1000;
		
		const uint weight_mutation_chance = 70 * 1000;
		const uint weight_perturb_chance = 90 * 1000;
		// probability that connection will get completely new weight
		const uint randomize_weight_chance = 10 * 1000;

		const uint add_neuron_chance = 3 * 1000;
		const uint add_connection_chance = 5 * 1000;
		// const uint delete_connection_chance

	public:
		// forward dec
		struct Const_Genome_Iterator;

		// creates Genotype with ONE random connection
		explicit Simplistic_Genotype(const size_t t_input_num,
			const size_t t_output_num);
		explicit Simplistic_Genotype(const size_t t_input_num,
			const size_t t_output_num, std::vector<Genome> t_genomes);
		virtual ~Simplistic_Genotype();

		Simplistic_Genotype(const Simplistic_Genotype& t_other);
		Simplistic_Genotype& operator=(Simplistic_Genotype t_other);

		// xml load and save
		void save_to_xml(pugi::xml_node t_xml_node);
		void load_from_xml(pugi::xml_node t_xml_node);

		friend void swap(Simplistic_Genotype& t_first,
			Simplistic_Genotype& t_second);

		explicit operator Generic_Genome();

		// neuron number of inputs = input_index
		const size_t input_num = 0;
		// neuron number of outputs = output_index + _input_num
		const size_t output_num = 0;
		const size_t bias_num = 1;
		const size_t fixed_neuron_num;

		std::string get_genomes_string() const;

		float get_fitness() const;
		void set_fitness(float t_fitness);

		// add new genome (not default mutation!)
		void add_genome(Genome t_genome);

		// distance to other genotype
		float distance(const Simplistic_Genotype& t_other);

		// decides which type of reproduction should be performed
		Reproduction_Type get_reproduction_type();

		// will create a new Simplistic_Genotype from two parents
		// the fitter parent serves as template, all excess and disjoint
		// genomes of other parent will be discarded
		// if both parents have the same fitness, excess and disjoint genes
		// are inherited randomly
		// weights of matching genes are averaged
		static Simplistic_Genotype create_offspring(
			const Simplistic_Genotype& t_first, const 
			Simplistic_Genotype& t_second);
		// mutate without crossover
		Simplistic_Genotype mutate(); 

		// returns true if should perform sexual reproduction
		bool perform_sexual_reproduction() const;

		size_t get_neuron_num() const;
		size_t get_hidden_neuron_num() const;
		// returns read-only Genome list
		const std::vector<Genome>& get_genome_list() const;
		// returns list of connected hidden neurons - needed for matching of 
		// genotype neuron num to phenotype neuron num
		const std::vector<size_t>& get_hidden_neurons() const;

	private:
		float _fitness = 0.0f;

		std::vector<Genome> _genomes;
		// holds every hidden neuron which is included in network
		// not sorted
		std::vector<size_t> _connected_hidden_neurons;

		// total global neuron num
		static size_t _global_neuron_num;

		void _delete_genomes();

		// register neuron - checks if already in _connected_hidden_neurons
		void _register_neuron(size_t t_neuron);

		// ######### mating & mutation ##################
		Simplistic_Genotype _mate(const Simplistic_Genotype& t_other,
			const bool t_same_fitness) const;

		// add next offspring genome if parents have same fitness
		void _next_same_fitness_mating_step(std::vector<Genome>& t_genomes,
			const Const_Genome_Iterator& t_iter)
			const;

		// add next offspring genome if parents have different fitness
		void _next_diff_fitness_mating_step(std::vector<Genome>& t_genomes,
			const Const_Genome_Iterator& t_iter)
			const;

		// returns true with probability of no_match_genome_chance
		bool _chance_add_genome() const;

		void _mutate_weight(Genome& t_genome);
		// mutation that adds new random connection
		// if by chance is false: connection is added in every case!
		void _mutate_new_conn();
		// mutation that adds new neuron by spliting existing connection
		void _mutate_new_neuron();

	public:
		// wrapps two genome lists and simplifies iteration
		// when reworking: do_on_same, do_on_first, do_on_second,
		// with std::optional as return
		struct Const_Genome_Iterator final
		{
		public:
			// enum for indicating where next genome is
			enum class Iterator_State
			{
				// in first/second list is genome with lower innovation num
				first_genome_list,
				second_genome_list,
				// genomes in both lists have same innovation num
				both_lists_equal
			};

			Const_Genome_Iterator(const std::vector<Genome>& t_first_genomes,
				const std::vector<Genome>& t_second_genomes);

			// returns true if reached end
			bool end() const;
			bool first_end_reached() const;
			bool second_end_reached() const;

			void advance();
			Iterator_State get_iterator_state() const;

			// gets next genome from first or second genome list respectively
			const Genome& first_genome() const;
			const Genome& second_genome() const;

		private:
			std::vector<Genome>::const_iterator _first_iter;
			std::vector<Genome>::const_iterator _second_iter;

			const std::vector<Genome>& _first_genomes;
			const std::vector<Genome>& _second_genomes;

			Iterator_State _iter_state;

			void _update_iter_state();
		};
	};
}


#endif // !SIMPLISTIC_GENOTYPE_HPP
