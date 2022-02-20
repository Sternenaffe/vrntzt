// test file

// TODO: replace similar variable names in the whole project

// original neat:
// https://github.com/F3R70/NEAT

ACTIVATION FN IS NOT IMPLEMENTED!!! FITNESS CANCELS OUT!

#include <iostream>
#include <vector>

#include "lib/IO/IO.hpp"

#include "include/Evolution/Neat_Evolution_Manager.hpp"
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

float get_xor(Simplistic_Phenotype& t_phenotype, float t_in1, float t_in2)
{
	t_phenotype.reset();
	t_phenotype.set_input(0, t_in1);
	t_phenotype.set_input(1, t_in2);
	t_phenotype.activate(10);

	return t_phenotype.get_output(0);
}

float eval_xor(Simplistic_Genotype* t_genotype)
{
	using decoding::decode;

	float fitness = 0;
	Simplistic_Phenotype phenotype =
		decode<Simplistic_Genotype, Simplistic_Phenotype>(*t_genotype);
	
	fitness += 1 - get_xor(phenotype, 0, 0);
	fitness += get_xor(phenotype, 0, 1);
	fitness += get_xor(phenotype, 1, 0);
	fitness += 1 - get_xor(phenotype, 1, 1);

	std::cout << "fitness: " << fitness << "\n";

	return fitness;
}

int main()
{
	decoding::init_simplistic_decode();

	// test
	Simplistic_Genotype test_genotype(2, 1);
	for (int i = 0; i < 10; ++i)
	{
		test_genotype = test_genotype.mutate();
	}
	eval_xor(&test_genotype);
	return 0;
	

	Neat_Evolution_Settings ev_settings;
	ev_settings.population_size = 20;
	Neat_Evolution_Manager<Simplistic_Genotype, Simplistic_Phenotype>
		ev_manager(2, 1, ev_settings);

	ev_manager.create_random_population();

	for (int i = 0; i < 10; ++i)
	{
		// get fitness
		for (auto& genotype : ev_manager.get_population())
		{

			genotype->set_fitness(eval_xor(genotype.get()));
		}

		IO::debug("\n================================================\n");
		IO::debug("Generation " + std::to_string(i) + "\n");

		ev_manager.evolve_population();

		for (auto& genotype : ev_manager.get_population())
		{
			IO::debug(genotype->get_genomes_string());
			IO::debug("\n");
		}
	}
	
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
