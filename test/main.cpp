// test file

#include <iostream>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/Decoding/Decoder.hpp"
#include "include/Decoding/Simplistic_Decoder.hpp"
#include "include/Genomes/Simplistic_Genotype.hpp"
#include "include/Phenomes/Simplistic_Phenotype.hpp"
//#include "../include/Generic_Genome.hpp"

using namespace vrntzt::neat;
using std::vector;

//struct decode_Simple_Genotype : public Simple_Genotype
//{
//	static Simple_Phenotype test_decode(Simple_Genotype& t_genome)
//	{
//		std::cout << "decode\n";
//		return Simple_Phenotype();
//	}
//};

int main()
{
	decoding::init_simplistic_decode();

	Simplistic_Genotype genotype1(2, 1);
	Simplistic_Genotype genotype2(2, 1);

	// offspring
	Simplistic_Genotype genotype3(2, 1);

	Simplistic_Phenotype test_ph = decoding::decode<Simplistic_Genotype,
		Simplistic_Phenotype>(genotype1);
	test_ph.set_input(0, 0);
	test_ph.set_input(1, 0);
	test_ph.activate(10);

	std::cout << "-------------------------------\n\n";

	// let genomes mutate
	for (int i = 0; i < 15; ++i)
	{
		genotype1.mutate();
		genotype2.mutate();
	}

	std::cout << "-------------------------------\n\n";

	// test crossover
	
	genotype1.set_fitness(1);

	for (int i = 0; i < 10; ++i)
	{
		genotype3 = Simplistic_Genotype::create_offspring(
			genotype1, genotype2);

		std::cout << "-------------------------------\n\n";
	}

	return 0;
}
