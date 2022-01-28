// test file

#include <iostream>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/Decoding/Decoder.hpp"
#include "include/Decoding/Simplistic_Decoder.hpp"
#include "include/Genomes/Simplistic_Genome.hpp"
#include "include/Phenomes/Simplistic_Phenome.hpp"
//#include "../include/Generic_Genome.hpp"

using namespace vrntzt::neat;
using std::vector;

//struct decode_simple_genome : public Simple_Genome
//{
//	static Simple_Phenome test_decode(Simple_Genome& t_genome)
//	{
//		std::cout << "decode\n";
//		return Simple_Phenome();
//	}
//};

int main()
{
	decoding::init_simplistic_decode();

	Genome g1(1, 3, 3.5);
	Genome g2(0, 3, -0.75);
	Genome g3(2, 3, 0.1);

	std::vector genome_vec{
		g1, g2
	};

	Simplistic_Genome genome(2, 1, genome_vec);
	genome.add_genome(g3);

	std::cout << "-------------------------------\n\n";

	for (int i = 0; i < 10; ++i)
	{
		Simplistic_Phenome net = decoding::decode<Simplistic_Genome,
			Simplistic_Phenome>(genome);

		net.set_input(0, 2);
		net.set_input(1, 1);

		net.activate(3);

		std::cout << net.get_output(0) << "\n";

		genome.mutate();

		std::cout << "-------------------------------\n\n";
	}

	return 0;
}
