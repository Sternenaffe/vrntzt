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

#ifndef VRNTZT_NEAT_C_DLL_EVOLUTION_WRAPPER_H
#define VRNTZT_NEAT_C_DLL_EVOLUTION_WRAPPER_H

#ifdef VRNTZT_NEAT_C_DLL_EXPORT
#define VRNTZT_NEAT_C_DLL_API __declspec(dllexport)
#else
#define VRNTZT_NEAT_C_DLL_API __declspec(dllimport)
#endif // VRNTZT_NEAT_C_DLL_EXPORT

#include "include/c_dll/simplistic_genotype_wrapper.h"

namespace vrntzt::neat::c_dll
{
	typedef unsigned int uint;

	// struct is only for type safety
	extern "C" typedef struct Neat_Evolution_Manager
	{
		// id represents index in internal storage
		void* handler;
	} Neat_Evolution_Manager;

	extern "C" typedef struct Neat_Evolution_Settings
	{
		uint population_size;
		uint species_count;
		float interspecies_mating_chance;
	} Neat_Evolution_Settings;

	// create wrapped Neat_Evolution_Manager with Simplistic_Genotype and
	// Simplistic_Phenotype
	extern "C" VRNTZT_NEAT_C_DLL_API Neat_Evolution_Manager new_neat_evolution_manager(
		uint t_inputs, uint t_outputs, Neat_Evolution_Settings t_settings);

	// delete existing Neat_Evolution_Manager and invalidates all corresponding
	// wrappers
	extern "C" VRNTZT_NEAT_C_DLL_API void delete_neat_evolution_manager(
		Neat_Evolution_Manager t_wrapped_manager);

	extern "C" VRNTZT_NEAT_C_DLL_API void create_random_population(
		Neat_Evolution_Manager t_wrapped_manager);

	extern "C" VRNTZT_NEAT_C_DLL_API void evolve_population(
		Neat_Evolution_Manager t_wrapped_manager);

	// need to pass array of Simplistic_Genotype_Wrapper with size = population_size
	extern "C" VRNTZT_NEAT_C_DLL_API void get_population(
		Neat_Evolution_Manager t_wrapped_manager, Simplistic_Genotype * t_target,
		size_t t_target_size);
}

#endif // !VRNTZT_NEAT_C_DLL_EVOLUTION_WRAPPER_H
