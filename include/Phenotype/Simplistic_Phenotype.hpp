// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT Header
// Simplistic Phenome

/* CHANGELOG
 *
 */

/* SUMMARY
 *
 * Simplistic Phenome Header File
 * 
 * most basic phenome - used for debugging purposes
 * 
 * internal connection list: sorted by target neuron; will be iterated over
 * order: bias - inputs - outputs - hidden
 * 
 * so neuron will have numbers in following range:
 * fixed neuron number = number of outputs + number of inputs + 1 (bias)
 * bias: 0
 * inputs: [1; number of inputs]
 * outputs: [number of inputs + 1; number of inputs + 1 + number of outputs]
 * hidden: [fixed neuron number + 1; fixed neuron number + hidden neuron number]
 */

// TODO: think about usage of size_t as network dimension
// TODO: policy of connections (valid source & target)
//		 should have internal sort/check
// TODO: replace get/set input/output with internal classes & slicing
// TODO: implement bound-checking
// TODO - IMPORTANT: pointer/reference for Connection array
// TODO: should use half instead of float (?)
// TODO: implement modular activation fn

#ifndef Simple_Phenotype_HPP
#define Simple_Phenotype_HPP

//#include "lib/utility/include/fixed_vector.hpp"

#include "include/vrntzt_global.hpp"
#include "include/Phenotype/IPhenotype.hpp"
#include "include/Generic_Genome.hpp"
#include "src/Helper/simple_helper.hpp"

namespace vrntzt::neat
{
	// debug settings
	constexpr bool SIMPLISTIC_PHENOTYPE_DEBUG = false;
	constexpr bool ACTIVATE_CONN_DEBUG = false;

	class Simplistic_Phenotype final : public IPhenotype<Simplistic_Phenotype>
	{
	public:
		// type which is used in network
		using internal_type = float;

		// non-sorted connections
		//explicit Simplistic_Phenotype(const Network_Dimensions t_net_dims,
			//const vector<Connection> t_connections);


		// constructors
		// IMPORTANT: t_connections must be sorted!
		explicit Simplistic_Phenotype(const Network_Dimensions t_net_dims,
			const std::vector<std::vector<Connection>> t_connections);

		explicit Simplistic_Phenotype(const Generic_Genome& genome);

		Simplistic_Phenotype(const Simplistic_Phenotype&) = delete;
		Simplistic_Phenotype& operator=(Simplistic_Phenotype) = delete;

		virtual ~Simplistic_Phenotype();

		// methods to get/set inputs & outputs
		void set_input(const int t_index, const internal_type t_value);
		internal_type get_input(const int t_index) const;
		internal_type get_output(const int t_index) const;

		// core function: will process inputs and store solution in output
		void activate(int t_iterations);

		void reset();

		// flags
		// check if activate is currently working
		bool is_working() const;

		// neuron number of inputs = input_index
		const size_t input_num = 0;
		// neuron number of outputs = output_index + _input_num
		const size_t output_num = 0;
		// always 1
		const size_t bias_num = 1;
		// fixed neuron num = _output_num + _input_num + bias_num
		// acts as offset for hidden neurons to give every neuron a
		// unique number
		const size_t fixed_neuron_num = 0;
		// neuron number = neuron_num + _fixed_neuron_number
		const size_t hidden_neuron_num = 0;

	private:
		// inits all members which have default values
		void _init_members();

		// sigmoid
		internal_type _activation_f(const internal_type t_value);

		void _update_neuron_state(const size_t t_neuron);

		// index represents source neuron;
		// _connections.size = input_num + hidden_neuron_num + bias_num
		const std::vector<std::vector<Connection>> _connections;

		// includes inputs, outputs and bias
		std::vector<internal_type> _neuron_states;
		// offset of inputs in _neuron_states = output_num + bias_num
		const size_t _output_offset = 0;

		// flags
		bool _working = false;
	};
}


#endif // !Simple_Phenotype_HPP
