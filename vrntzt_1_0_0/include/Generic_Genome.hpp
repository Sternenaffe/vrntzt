// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// Generic Genome

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * Generic Genome Header
  * 
  * The Generic_Genome is used for conversion between Genome and Phenome where no
  * direct conversion exists. It's a compromise between multifunctional information
  * and fast decoding and encoding.
  * can't be used as regular genome class
  * 
  */

#ifndef GENERIC_GENOME_HPP
#define GENERIC_GENOME_HPP


namespace vrntzt::neat
{
	class Generic_Genome
	{
		/*
		 *
		 */

		// delete unneeded copy & assignment
		Generic_Genome(const Generic_Genome&) = delete;
		Generic_Genome& operator=(const Generic_Genome&) = delete;

	public:
		Generic_Genome() {}
	};
}

#endif // !GENERIC_GENOME_HPP
