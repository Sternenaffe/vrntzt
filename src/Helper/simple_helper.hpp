// ch 2021
// c++20
// v1.0.0
// vrntzt
// neat
// helper header for simple/simplistic

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * declares structs etc. which will be used both by genome
  * and phenome
  */

#ifndef VRNTZT_SIMPLE_HELPER_HPP
#define VRNTZT_SIMPLE_HELPER_HPP

#include "lib/utility/include/trivial_typedefs.hpp"

namespace vrntzt::neat
{
	struct Network_Dimensions
	{
	public:
		size_t input_num = 0;
		size_t output_num = 0;
		size_t hidden_neuron_num = 0;

		// returns input_num + output_num
		size_t fixed_neuron_num() const;
	};

	struct Connection
	{
	public:
		Connection(const size_t t_source_neuron,
			const size_t t_target_neuron,
			float t_weight);
		// creates connection with random weight
		Connection(const size_t t_source_neuron,
			const size_t t_target_neuron) = delete;

		Connection(const Connection& t_other);
		//Connection& operator=(Connection t_other) = delete;

		// members are not const because of assignment operator!
		// includes input and output neurons
		size_t source_neuron = 0;
		// target is not really needed in this context cause corresponds
		// to vector index
		size_t target_neuron = 0;
		float weight = 0.0;
	};
}

#endif // !VRNTZT_SIMPLE_HELPER_HPP
