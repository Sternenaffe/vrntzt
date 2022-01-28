// ch 2021
// c++20
// v1.0.0
// vrntzt
// neat
// helper source for simple/simplistic

#include "include/Helper/simple_helper.hpp"

namespace vrntzt::neat
{
	Connection::Connection(const ushort t_source_neuron,
		const ushort t_target_neuron,
		float t_weight)
		:
		source_neuron(t_source_neuron),
		target_neuron(t_target_neuron)
	{
		weight = t_weight;
	}

	Connection::Connection(const Connection& t_other)
		:
		source_neuron(t_other.source_neuron),
		target_neuron(t_other.target_neuron)
	{
		weight = t_other.weight;
	}

	Connection& Connection::operator=(Connection)
	{
		return *this;
	}
}