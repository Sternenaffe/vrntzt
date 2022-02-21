// ch 2021-22
// c++20
// v1.0.0
// vrntzt
// neat
// decoder extension header

#include "include/Decoding/Simplistic_Decoder.hpp"

#include <vector>

#include "lib/IO/IO.hpp"

#include "include/Genotype/Simplistic_Genotype.hpp"
#include "include/Phenotype/Simplistic_Phenotype.hpp"

using std::vector;

namespace vrntzt::neat::decoding
{
	void init_simplistic_decode()
	{
		if constexpr (SIMPLISTIC_DECODING_DEBUG)
		{
			IO::debug("init simplistic genome decode\n");
		}

		set_decode_function<Simplistic_Genotype, Simplistic_Phenotype>(
			&decode_Simplistic_Genotype);
	}

	vector<vector<Connection>> get_sorted_connections(Simplistic_Genotype& t_genotype)
	{
		// should not contain inputs and bias
		vector<vector<Connection>> temp_vec;
		ushort max_neuron_num = 0;

		for (const Genome& genome : t_genotype.get_genome_list())
		{
			if (genome.connection.target_neuron <
				(t_genotype.input_num + t_genotype.bias_num))
			{
				IO::error("decode simplistic: target neuron can't be input "
					"or bias!");
			}

			// find max neuron number
			// REALLY BAD SOLUTION
			max_neuron_num = std::max(max_neuron_num, genome.connection.source_neuron);
			max_neuron_num = std::max(max_neuron_num, genome.connection.target_neuron);

			if (max_neuron_num >= temp_vec.size())
			{
				temp_vec.resize(max_neuron_num + 1);
			}

			// calculate pos in connection vector
			// vector stores connections to hidden neurons and outputs
			ushort index = genome.connection.target_neuron -
				t_genotype.input_num - t_genotype.bias_num;

			temp_vec[index].push_back(genome.connection);
		}

		return temp_vec;
	}

	Simplistic_Phenotype decode_Simplistic_Genotype(Simplistic_Genotype& t_genotype)
	{
		if constexpr (SIMPLISTIC_DECODING_DEBUG)
		{
			IO::debug("decode Simplistic_Genotype to Simplistic_Phenotype\n");
		}
		
		vector<vector<Connection>> sorted_connections =
			get_sorted_connections(t_genotype);
		Network_Dimensions dim;
		dim.input_num = t_genotype.input_num;
		dim.output_num = t_genotype.output_num;
		dim.hidden_neuron_num = std::max(0u,
			sorted_connections.size() - t_genotype.output_num);

		return Simplistic_Phenotype(dim, sorted_connections);
	}
}
