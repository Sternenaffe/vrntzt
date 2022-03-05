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

#include "include/c_dll/wrapping_helper.hpp"

namespace vrntzt::neat::c_dll
{
	Internal_Neat_Evolution_Settings _convert_ev_settings(
		Neat_Evolution_Settings_Wrapper t_settings)
	{
		Internal_Neat_Evolution_Settings settings;
		settings.population_size = t_settings.population_size;
		settings.species_count = t_settings.species_count;
		settings.interspecies_mating_chance = t_settings.interspecies_mating_chance;

		return settings;
	}

	Neat_Evolution_Manager_Wrapper _wrap_ev_manager(
		Internal_Simplistic_Neat_Evolution_Manager* t_manager)
	{
		// wrap evolution manager
		Neat_Evolution_Manager_Wrapper wrapped_manager(
			static_cast<void*>(t_manager));
		return wrapped_manager;
	}

	Internal_Simplistic_Neat_Evolution_Manager* _unwrap_ev_manager(
		Neat_Evolution_Manager_Wrapper t_wrapper)
	{
		return static_cast<Internal_Simplistic_Neat_Evolution_Manager*>(t_wrapper.handler);
	}

	Simplistic_Genotype_Wrapper _wrap_simplistic_genotype(
		Internal_Simplistic_Genotype* t_genotype)
	{
		// wrap simplistic genotype
		Simplistic_Genotype_Wrapper wrapped_genotype(
			static_cast<void*>(t_genotype));
		return wrapped_genotype;
	}

	Internal_Simplistic_Genotype* _unwrap_simplistic_genotype(
		Simplistic_Genotype_Wrapper t_wrapper)
	{
		return static_cast<Internal_Simplistic_Genotype*>(t_wrapper.handler);;
	}
}