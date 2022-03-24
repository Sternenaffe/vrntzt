// test file

// TODO: consider use of std::optional
// TODO: replace similar variable names in the whole project
// TODO: exclude second bound when getting random integers from Random_Generator

// original neat:
// https://github.com/F3R70/NEAT

#include <iostream>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/vrntzt_global.hpp"
#include "include/Evolution/Neat_Evolution_Manager.hpp"
#include "include/Decoding/Decoder.hpp"
#include "include/Decoding/Simplistic_Decoder.hpp"
#include "include/Genotype/Simplistic_Genotype.hpp"
#include "include/Phenotype/Simplistic_Phenotype.hpp"
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

float get_xor(Simplistic_Phenotype& t_phenotype, float t_in1, float t_in2)
{
	t_phenotype.reset();
	t_phenotype.set_input(0, t_in1);
	t_phenotype.set_input(1, t_in2);

	t_phenotype.activate(3);

	return t_phenotype.get_output(0);
}

float eval_xor(Simplistic_Genotype* t_genotype)
{
	using decoding::decode;

	float fitness = 0;
	Simplistic_Phenotype phenotype =
		decode<Simplistic_Genotype, Simplistic_Phenotype>(*t_genotype);
	
	float out0 = get_xor(phenotype, 0, 0);
	float out1 = get_xor(phenotype, 0, 1);
	float out2 = get_xor(phenotype, 1, 0);
	float out3 = get_xor(phenotype, 1, 1);

	// odd exponent because of negative numbers
	fitness += pow(1 - out0, 5);
	fitness += 1 + pow(out1 - 1, 5);
	fitness += 1 + pow(out2 - 1, 5);
	fitness += pow(1 - out3, 5);

	//fitness = pow(fitness, 4);

	// bounty for having every output in right half
	if (out0 < 0.5f && out1 > 0.5f && out2 > 0.5f && out3 < 0.5f)
	{
		fitness += 10;
	}

	// std::cout << "fitness: " << fitness << "\n";

	return fitness;
}

int main()
{
	decoding::init_simplistic_decode();

	//// test
	//Simplistic_Genotype test_genotype(2, 1);
	//for (int i = 0; i < 10; ++i)
	//{
	//	test_genotype = test_genotype.mutate();
	//}
	//eval_xor(&test_genotype);
	//return 0;

	Neat_Evolution_Settings ev_settings =
	{
		.population_size = 50,
		.species_count = 10	
	};

	Neat_Evolution_Manager<Simplistic_Genotype, Simplistic_Phenotype>
		ev_manager(2, 1, ev_settings);

	uint generation_avg = 0;

	for (uint i = 0; i < 10; ++i)
	{
		uint generation = 0;

		ev_manager.create_random_population();

		//for (int i = 0; i < 1000; ++i)
		while (true)
		{
			++generation;

			// get fitness
			for (auto& genotype : ev_manager.get_population())
			{
				//std::cout << "start fitness: " << genotype->get_fitness() << "\n";
				// bottleneck - i guess sorting in decoding
				genotype->set_fitness(eval_xor(genotype.get()));
				//genotype->set_fitness(0);
			}

			auto best_genotype = ev_manager.get_previous_best_genotype();

			IO::out("\nbest genotype fitness: " + std::to_string(best_genotype->get_fitness()) + "\n");

			if (best_genotype->get_fitness() > 13.9)
			{
				ev_manager.save("xor_solution.xml");
				//return 0;
				break;
			}

			IO::out("\n================================================\n");
			IO::out("Generation " + std::to_string(generation) + "\n");

			ev_manager.evolve_population();
		}

		generation_avg += generation;
	}

	std::cout << "\n\n\nAverage Generations: " << generation_avg / 10 << "\n";
	
	return 0;

	/*
	// offspring
	// Simplistic_Genotype genotype3(2, 1);

	Simplistic_Phenotype test_ph = decoding::decode<Simplistic_Genotype,
		Simplistic_Phenotype>(genotype1);
	test_ph.set_input(0, 0);
	test_ph.set_input(1, 0);
	test_ph.activate(10);

	std::cout << "-------------------------------\n\n";

	// let genomes mutate & mate
	for (int i = 0; i < 1'000; ++i)
	{
		genotype1.mutate();
		genotype2.mutate();
		genotype1 = Simplistic_Genotype::create_offspring(
			genotype1, genotype2);
		genotype2.distance(genotype1);
		genotype1.distance(genotype2);
	}

	std::cout << "-------------------------------\n\n";

	return 0;*/
}
