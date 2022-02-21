// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT Header
// Simple Phenotype

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * Simple Phenome Header File
  * 
  * most simple phenome
  */

#ifndef Simple_Phenotype_HPP
#define Simple_Phenotype_HPP

#include "include/Phenotype/IPhenotype.hpp"
#include "include/Generic_Genome.hpp"

namespace vrntzt::neat
{
	class Simple_Phenotype : public IPhenotype<Simple_Phenotype>
	{
	public:
		// constructors
		explicit Simple_Phenotype();
		explicit Simple_Phenotype(const Generic_Genome& genome);

		virtual ~Simple_Phenotype();

		
	};
}


#endif // !Simple_Phenotype_HPP
