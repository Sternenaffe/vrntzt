// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT source
// Simple Genome

#include "lib/pugixml-1.12/src/pugixml.hpp"

#include "include/vrntzt_pch.h"
#include "include/Genotype/Simple_Genotype.hpp"

namespace vrntzt::neat
{
	Simple_Genotype::Simple_Genotype()
	{

	}

	Simple_Genotype::~Simple_Genotype()
	{

	}

	Simple_Genotype::operator Generic_Genome()
	{
		return Generic_Genome();
	}
}
