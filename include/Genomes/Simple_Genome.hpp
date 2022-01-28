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


#ifndef SIMPLE_GENOME_HPP
#define SIMPLE_GENOME_HPP

#include "include/Genomes/IGenome.hpp"
#include "include/Generic_Genome.hpp"

namespace vrntzt::neat
{
	class Simple_Genome : public IGenome<Simple_Genome>
	{
	public:
		explicit Simple_Genome();
		virtual ~Simple_Genome();

		explicit operator Generic_Genome();
	};
}


#endif // !SIMPLE_GENOME_HPP
