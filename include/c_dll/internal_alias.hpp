// ch 2021-22
// c
// v1.0.0
// vrntzt
// neat
// c-dll cpp typedefs

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_NEAT_C_DLL_INTERNAL_ALIAS_HPP
#define VRNTZT_NEAT_C_DLL_INTERNAL_ALIAS_HPP

#include "include/c_dll/neat_evolution_wrapper.h"
#include "include/c_dll/simplistic_genotype_wrapper.h"
#include "include/c_dll/simplistic_phenotype_wrapper.h"
#include "include/Evolution/Neat_Evolution_Manager.hpp"
#include "include/Genotype/Simplistic_Genotype.hpp"
#include "include/Phenotype/Simplistic_Phenotype.hpp"

namespace vrntzt::neat::c_dll
{
	// create internal alias to avoid name confusion
	using Simplistic_Genotype_Wrapper = vrntzt::neat::c_dll::Simplistic_Genotype;
	using Internal_Simplistic_Genotype = vrntzt::neat::Simplistic_Genotype;

	using Neat_Evolution_Settings_Wrapper = vrntzt::neat::c_dll::Neat_Evolution_Settings;
	using Internal_Neat_Evolution_Settings = vrntzt::neat::Neat_Evolution_Settings;

	using Internal_Simplistic_Phenotype = vrntzt::neat::Simplistic_Phenotype;
	using Simplistic_Phenotype_Wrapper = vrntzt::neat::c_dll::Simplistic_Phenotype;

	using Neat_Evolution_Manager_Wrapper = vrntzt::neat::c_dll::Neat_Evolution_Manager;
	using Internal_Simplistic_Neat_Evolution_Manager = vrntzt::neat::Neat_Evolution_Manager<
		Internal_Simplistic_Genotype, Internal_Simplistic_Phenotype>;
}

#endif // !VRNTZT_NEAT_C_DLL_INTERNAL_ALIAS_HPP
