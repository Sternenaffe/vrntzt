// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT Header
// Simple Genome

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * Simple Genome Header File
  * 
  * genome: stores network neuron numbers (not global
  * numbers!) to provide continuous neuron numbering and
  * remove unused in current net. provide transform list
  * which stores global neuron number of global neuron
  * (not reverse, cause more difficult transform)
  * 
  * store global neuron number in ev algo?
  */


#ifndef Simple_Genotype_HPP
#define Simple_Genotype_HPP

#include "include/Genomes/IGenotype.hpp"
#include "include/Generic_Genome.hpp"

namespace vrntzt::neat
{
	class Simple_Genotype : public IGenotype<Simple_Genotype>
	{
	public:
		explicit Simple_Genotype();
		virtual ~Simple_Genotype();

		explicit operator Generic_Genome();
	};
}


#endif // !Simple_Genotype_HPP
