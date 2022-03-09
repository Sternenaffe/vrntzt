// ch 2021-22
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat simplistic genotype wrapping header

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

#include "include/c_dll/simplistic_phenotype_wrapper.h"

namespace vrntzt::neat::c_dll
{
	// struct is only for type safety
	extern "C" struct Simplistic_Genotype
	{
		void* handler;
	};

	extern "C" VRNTZT_NEAT_C_DLL_API
	float get_fitness(Simplistic_Genotype t_wrapper);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_fitness(Simplistic_Genotype t_wrapper, float t_fitness);

	extern "C" VRNTZT_NEAT_C_DLL_API
	Simplistic_Phenotype decode(Simplistic_Genotype t_wrapper);
}

#endif // !VRNTZT_NEAT_C_DLL_SIMPLISTIC_GENOTYPE_WRAPPER_H
