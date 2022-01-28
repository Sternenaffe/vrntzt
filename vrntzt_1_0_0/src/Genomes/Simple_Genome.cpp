// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simple Genome

#include "include/Genomes/Simple_Genome.hpp"

namespace vrntzt::neat
{
	Simple_Genome::Simple_Genome()
	{

	}

	Simple_Genome::~Simple_Genome()
	{

	}

	Simple_Genome::operator Generic_Genome()
	{
		return Generic_Genome();
	}
}
