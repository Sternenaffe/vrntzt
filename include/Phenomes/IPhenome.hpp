// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// Phenome Interface

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * phenome interface for neat learning

  * phenome: representation of a neuronal network; optimized for activation (performance)
  *
  * interface functions:
  * -
  */

#ifndef I_PHENOME_HPP
#define I_PHENOME_HPP

#include "lib/utility/include/has_member.hpp"

#include "include/Generic_Genome.hpp"
// forward dec
//class Generic_Genome;

namespace vrntzt::neat
{
	template <class Child_Phenome>
	class IPhenome
	{
		/* Phenome interface
		 *
		 * checks for certain interface functions (when constructing)
		 *
		 * interface functions :
		 * - encode<Generic_Genome>() : encoder to Generic_Genome, used as
		 *		intermediate step to encode to Genome where no direct
		 *		specified function exists
		 *
		 */

	protected:
		// not directly constructable
		explicit IPhenome()
		{
			// interface checks
			// must be constructable from Generic_Genome to enable default decode
			static_assert(u_lib::constructible_with<Child_Phenome, Generic_Genome>::value,
				"Derived class of 'IPhenome' needs to provide a constructor "
				"from 'Generic_Genome'"
				);
		}

		virtual ~IPhenome() {}

		
	};
}

#endif // !I_PHENOME_HPP
