// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// Genome Interface

/* CHANGELOG
 * 
 */

/* SUMMARY
 * 
 * genome interface for neat learning
 * 
 * genome: encoded representation of a neuronal network; optimized for
 *         storage, mutation, crossing and speciation
 * 
 * API of Genomes:
 * 
 * Target_Phenome decode<Target_Phenome>():
 *		Target_Phenome must inherit from IPhenome or be Generic_Genome
 * 
 * 
 * 
 * should add convert<Target_Genome> function? will have information loss
 */

#ifndef I_GENOME_HPP
#define I_GENOME_HPP

#include "lib/utility/include/has_member.hpp"

#include "src/vrntzt_concepts.hpp"

namespace vrntzt::neat
{
	// forward dec
	// Generic_Phenome & Generic_Genome: abstracted (not usable) generic
	// representations
	class Generic_Genome;

	// interface class for every Genome
	template <class Child_Genome>
	class IGenome
	{
		/* Genome interface
		 * 
		 * checks for certain interface functions (when constructing);
		 * can't be constructed directely (interface with protected constructor)
		 * 
		 * interface functions :
		 * - decode<Generic_Genome>() : decoder to Generic_Genome, used as
		 *		intermediate step to decode to Phenome where no direct
		 *		specified function exists
		 *		
		 * Genomes should not be marked final and all important attributes
		 * protected or accessable through protected getters. this enables
		 * custom decode functions to access all information they need
		 */
		
	protected:
		explicit IGenome()
		{
			// needs to support conversion to Generic_Genome to enable default
			// decoding
			static_assert(u_lib::has_member_function<Child_Genome,
				&Child_Genome::operator Generic_Genome, Generic_Genome>::value,
				"Derived class of 'IGenome' needs to provide a conversion "
				"operator to 'Generic_Genome'"
				);
		}

		virtual ~IGenome() {};
	};
}

#endif // !I_GENOME_HPP
