// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simplistic Phenome

#include <vector>
#include <sstream>

#include "lib/IO/IO.hpp"

#include "include/Phenomes/Simplistic_Phenome.hpp"

namespace vrntzt::neat
{
	using std::vector;
	using internal_type = Simplistic_Phenome::internal_type;

	ushort Network_Dimensions::fixed_neuron_num() const
	{
		return input_num + output_num;
	}

	Simplistic_Phenome::Simplistic_Phenome(
		const Network_Dimensions t_net_dims,
		const vector<vector<Connection>> t_connections)
		:
		// init base
		IPhenome<Simplistic_Phenome>(),
		input_num(t_net_dims.input_num),
		output_num(t_net_dims.output_num),
		// bias_num is always 1
		fixed_neuron_num(t_net_dims.fixed_neuron_num() + bias_num),
		hidden_neuron_num(t_net_dims.hidden_neuron_num),
		_connections(t_connections),
		_output_offset(t_net_dims.input_num + bias_num)
	{
		IO::debug("custom init of Simplistic_Phenome\n");

		if (_connections.size() != output_num + hidden_neuron_num)
		{
			IO::error("invalid connection list size!");
		}

		_init_members();
	}

	// not ready for use!
	Simplistic_Phenome::Simplistic_Phenome(
		const Generic_Genome& genome)
		:
		IPhenome<Simplistic_Phenome>(),
		input_num(0),
		output_num(0),
		fixed_neuron_num(0),
		hidden_neuron_num(0),
		_connections(vector<vector<Connection>>())
	{
		IO::debug("Init Simplistic_Phenome from Generic_Genome\n");
		IO::error("Not supported yet!\n");
		_init_members();
	}

	Simplistic_Phenome::~Simplistic_Phenome()
	{

	}

	void Simplistic_Phenome::_init_members()
	{
		_neuron_states = vector<internal_type>(fixed_neuron_num +
			hidden_neuron_num);
	}

	void Simplistic_Phenome::set_input(const int t_index,
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

	internal_type Simplistic_Phenome::get_input(const int t_index) const
	{
		return _neuron_states[t_index + bias_num];
	}

	internal_type Simplistic_Phenome::get_output(const int t_index) const
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

	void Simplistic_Phenome::activate(int t_iterations)
	{
		IO::debug("activate network\n");

		// set bias
		_neuron_states[0] = 1.0;

		// should clear up variable names

		// set flag
		_working = true;

		// can be cyclic -> specified number of iterations
		for (int i = 0; i < t_iterations; ++i)
		{
			// iterate over all neurons except inputs and bias which
			// won't change state
			for (int current_neuron = _output_offset;
				current_neuron < fixed_neuron_num + hidden_neuron_num;
				++current_neuron)
			{
				_update_neuron_state(current_neuron);
			}
		}
	
		_working = false;
	}

	bool Simplistic_Phenome::is_working() const
	{
		return _working;
	}

	void Simplistic_Phenome::_update_neuron_state(const ushort t_neuron)
	{
		// connections vector only contains hidden_neurons and outputs
		size_t connections_index = t_neuron - _output_offset;

		const vector<Connection>& conn_to_neuron = _connections[connections_index];

		// store neuron state in case it is connected to itself
		internal_type prev_state = _neuron_states[t_neuron];

		// clear state
		_neuron_states[t_neuron] = 0;

		// iterate over all connections and add to neuron state
		for (Connection connection : conn_to_neuron)
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
	}
}
