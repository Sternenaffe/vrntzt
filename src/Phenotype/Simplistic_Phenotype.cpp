// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simplistic Phenome

#include "include/vrntzt_pch.h"

#include <vector>
#include <sstream>
#include <cmath>

#include "lib/IO/IO.hpp"

#include "include/Phenotype/Simplistic_Phenotype.hpp"

namespace vrntzt::neat
{
	using std::vector;
	using internal_type = Simplistic_Phenotype::internal_type;

	size_t Network_Dimensions::fixed_neuron_num() const
	{
		return input_num + output_num;
	}

	Simplistic_Phenotype::Simplistic_Phenotype(
		const Network_Dimensions t_net_dims,
		const vector<vector<Connection>> t_connections)
		:
		// init base
		IPhenotype<Simplistic_Phenotype>(),
		input_num(t_net_dims.input_num),
		output_num(t_net_dims.output_num),
		// bias_num is always 1
		fixed_neuron_num(t_net_dims.fixed_neuron_num() + bias_num),
		hidden_neuron_num(t_net_dims.hidden_neuron_num),
		_connections(t_connections),
		_output_offset(t_net_dims.input_num + bias_num)
	{
		if constexpr (SIMPLISTIC_PHENOTYPE_DEBUG)
		{
			IO::debug("custom init of Simplistic_Phenotype\n");
		}

		/*if (_connections.size() != output_num + hidden_neuron_num)
		{
			IO::error("invalid connection list size!");
		}*/

		_init_members();
	}

	// not ready for use!
	Simplistic_Phenotype::Simplistic_Phenotype(
		const Generic_Genome& genome)
		:
		IPhenotype<Simplistic_Phenotype>(),
		input_num(0),
		output_num(0),
		fixed_neuron_num(0),
		hidden_neuron_num(0),
		_connections(vector<vector<Connection>>())
	{
		if constexpr (SIMPLISTIC_PHENOTYPE_DEBUG)
		{
			IO::debug("Init Simplistic_Phenotype from Generic_Genome\n");
		}

		IO::error("Not supported yet!\n");
		_init_members();
	}

	Simplistic_Phenotype::~Simplistic_Phenotype()
	{

	}

	void Simplistic_Phenotype::_init_members()
	{
		_neuron_states = vector<internal_type>(fixed_neuron_num +
			hidden_neuron_num);
	}

	internal_type Simplistic_Phenotype::_activation_f(const internal_type t_value)
	{
		// values taken from stanley.ec02.pdf (MIT paper)
		// return 1 / (1 + exp(-4.9f * t_value));
		
		// fast sigmoid function
		// matches MIT paper sigmoid close enough, but faster
		internal_type offset = 1;
		internal_type vertical_scale = 0.5f;
		internal_type horizontal_scale = 4;

		internal_type scaled_value = t_value * horizontal_scale;
		return vertical_scale * (scaled_value / (1 + abs(scaled_value)) + offset);
	}

	void Simplistic_Phenotype::set_input(const size_t t_index,
		const internal_type t_value)
	{
		if (t_index < input_num && t_index >= 0)
		{
			_neuron_states[t_index + bias_num] = t_value;
		}
		else
		{
			IO::error("input index out of range!");
		}
	}

	internal_type Simplistic_Phenotype::get_input(const size_t t_index) const
	{
		return _neuron_states[t_index + bias_num];
	}

	internal_type Simplistic_Phenotype::get_output(const size_t t_index) const
	{
		if (t_index < output_num && t_index >= 0)
		{
			return _neuron_states[_output_offset + t_index];
		}
		else
		{
			IO::error("output index out of range!");
			return 0;
		}
	}

	void Simplistic_Phenotype::activate(uint t_iterations)
	{
		if constexpr (SIMPLISTIC_PHENOTYPE_DEBUG)
		{
			IO::debug("activate network\n");
		}

		// set bias
		_neuron_states[0] = 1.0;

		// should clear up variable names

		// set flag
		_working = true;

		// can be cyclic -> specified number of iterations
		for (size_t i = 0; i < t_iterations; ++i)
		{
			// iterate over all neurons except inputs and bias which
			// won't change state
			for (size_t current_neuron = _output_offset;
				current_neuron < fixed_neuron_num + hidden_neuron_num;
				++current_neuron)
			{
				_update_neuron_state(current_neuron);
			}
		}
	
		_working = false;
	}

	void Simplistic_Phenotype::reset()
	{
		// clear internal states - not necessary for feedforward nets
		for (auto& state : _neuron_states)
		{
			state = 0;
		}
	}

	bool Simplistic_Phenotype::is_working() const
	{
		return _working;
	}

	void Simplistic_Phenotype::_update_neuron_state(const size_t t_neuron)
	{
		// connections vector only contains hidden_neurons and outputs
		size_t connections_index = t_neuron - _output_offset;

		const vector<Connection>& conn_to_neuron = _connections[connections_index];

		// store neuron state in case it is connected to itself
		internal_type prev_state = _neuron_states[t_neuron];

		// clear state
		_neuron_states[t_neuron] = 0;

		// iterate over all connections and add to neuron state
		for (const Connection& connection : conn_to_neuron)
		{
			// temp variable for debugging
			internal_type temp_add = 0;

			// check if connection to itself
			if (connection.source_neuron == t_neuron)
			{
				 temp_add = prev_state * connection.weight;
			}
			else
			{
				temp_add = _neuron_states[connection.source_neuron] * \
					connection.weight;
			}

			if constexpr (ACTIVATE_CONN_DEBUG)
			{
				std::ostringstream temp_string;
				temp_string << "Connection" <<
					"\nFrom: " << connection.source_neuron <<
					"\nTo: " << t_neuron <<
					"\nWeight: " << connection.weight << "\n\n";
				IO::debug(temp_string.str());
			}

			_neuron_states[t_neuron] += temp_add;
		}

		// apply activation function
		_neuron_states[t_neuron] = _activation_f(_neuron_states[t_neuron]);
	}
}
