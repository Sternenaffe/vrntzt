// ch 2021-22
// c++20
// v1.0.0
// vrntzt
// neat
// decoder extension header

#include "include/Decoding/Simplistic_Decoder.hpp"

#include <vector>

#include "lib/IO/IO.hpp"

#include "include/Genomes/Simplistic_Genome.hpp"
#include "include/Phenomes/Simplistic_Phenome.hpp"

using std::vector;

namespace vrntzt::neat::decoding
{
	void init_simplistic_decode()
	{
		IO::debug("init simplistic genome decode\n");

		set_decode_function<Simplistic_Genome, Simplistic_Phenome>(
			&decode_simplistic_genome);
	}

	vector<vector<Connection>> get_sorted_connections(Simplistic_Genome& t_genome)
	{
		// should not contain inputs and bias
		vector<vector<Connection>> temp_vec(t_genome.get_neuron_num()
			- t_genome.input_num - t_genome.bias_num);

		for (const Genome& genome : t_genome.get_genome_list())
		{
			if (genome.connection.target_neuron <
				(t_genome.input_num + t_genome.bias_num))
			{
				IO::error("decode simplistic: target neuron can't be input "
					"or bias!");
			}

			// calculate pos in connection vector
			// vector stores connections to hidden neurons and outputs
			ushort index = genome.connection.target_neuron -
				t_genome.input_num - t_genome.bias_num;
			
			temp_vec[index].push_back(genome.connection);
		}

		return temp_vec;
	}

	Simplistic_Phenome decode_simplistic_genome(Simplistic_Genome& t_genome)
	{
		IO::debug("decode Simplistic_Genome to Simplistic_Phenome\n");

		vector<vector<Connection>> sorted_connections = get_sorted_connections(t_genome);
		Network_Dimensions dim;
		dim.input_num = t_genome.input_num;
		dim.output_num = t_genome.output_num;
		dim.hidden_neuron_num = t_genome.get_hidden_neuron_num();

		return Simplistic_Phenome(dim, sorted_connections);
	}
}
