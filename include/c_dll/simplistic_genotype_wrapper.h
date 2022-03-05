// ch 2021
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat evolution wrapping header

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  *
  */

  // TODO: use type erasure in order to store managers with different genotypes 
  //		 or phenotypes

#ifndef VRNTZT_NEAT_C_DLL_SIMPLISTIC_GENOTYPE_WRAPPER_H
#define VRNTZT_NEAT_C_DLL_SIMPLISTIC_GENOTYPE_WRAPPER_H

#ifdef VRNTZT_NEAT_C_DLL_EXPORT
#define VRNTZT_NEAT_C_DLL_API __declspec(dllexport)
#else
#define VRNTZT_NEAT_C_DLL_API __declspec(dllimport)
#endif // VRNTZT_NEAT_C_DLL_EXPORT

namespace vrntzt::neat::c_dll
{
	// struct is only for type safety
	extern "C" struct Simplistic_Genotype
	{
		// id represents key in internal map
		void* handler;
	};
}

#endif // !VRNTZT_NEAT_C_DLL_SIMPLISTIC_GENOTYPE_WRAPPER_H
