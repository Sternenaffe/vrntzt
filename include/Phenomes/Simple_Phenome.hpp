// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT Header
// Simple Phenome

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * Simple Phenome Header File
  * 
  * most simple phenome
  */

#ifndef SIMPLE_PHENOME_HPP
#define SIMPLE_PHENOME_HPP

#include "include/Phenomes/IPhenome.hpp"
#include "include/Generic_Genome.hpp"

namespace vrntzt::neat
{
	class Simple_Phenome : public IPhenome<Simple_Phenome>
	{
	public:
		// constructors
		explicit Simple_Phenome();
		explicit Simple_Phenome(const Generic_Genome& genome);

		virtual ~Simple_Phenome();

		
	};
}


#endif // !SIMPLE_PHENOME_HPP
