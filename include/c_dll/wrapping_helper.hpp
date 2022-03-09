// ch 2021
// c
// v1.0.0
// vrntzt
// neat
// c-dll helper which wraps cpp objects into c structs

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_NEAT_C_DLL_WRAPPING_HELPER_HPP
#define VRNTZT_NEAT_C_DLL_WRAPPING_HELPER_HPP

#include "include/c_dll/internal_alias.hpp"

namespace vrntzt::neat::c_dll
{
	// forward dec
	Internal_Neat_Evolution_Settings _convert_ev_settings(
		Neat_Evolution_Settings_Wrapper t_settings);

	// wraps a Neat_Evolution_Manager object into a c-style struct
	Neat_Evolution_Manager_Wrapper _wrap_ev_manager(
		Internal_Simplistic_Neat_Evolution_Manager* t_manager);
	// uwraps a Neat_Evolution_Manager object
	Internal_Simplistic_Neat_Evolution_Manager* _unwrap_ev_manager(
		const Neat_Evolution_Manager_Wrapper& t_wrapper);

	// wraps a Simplistic_Genotype object into a c-style struct
	Simplistic_Genotype_Wrapper _wrap_simplistic_genotype(
		Internal_Simplistic_Genotype* t_genotype);
	// uwraps a Simplistic_Genotype object
	Internal_Simplistic_Genotype* _unwrap_simplistic_genotype(
		const Simplistic_Genotype_Wrapper& t_wrapper);

	// wraps a Simplistic_Phenotype object into a c-style struct
	Simplistic_Phenotype_Wrapper _wrap_simplistic_phenotype(
		Internal_Simplistic_Phenotype* t_phenotype);
	// uwraps a Simplistic_Phenotype object
	Internal_Simplistic_Phenotype* _unwrap_simplistic_phenotype(
		const Simplistic_Phenotype_Wrapper& t_wrapper);
}

#endif // !VRNTZT_NEAT_C_DLL_WRAPPING_HELPER_HPP
