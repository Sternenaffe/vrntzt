// ch 2021
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat evolution wrapping header

#include "include/vrntzt_pch.h"
#define VRNTZT_NEAT_C_DLL_EXPORT
#include "include/c_dll/neat_evolution_wrapper.h"

#include "lib/utility/include/trivial_typedefs.hpp"

#include "include/c_dll/internal_alias.hpp"
#include "include/c_dll/wrapping_helper.hpp"
#include "include/Evolution/Neat_Evolution_Manager.hpp"

namespace vrntzt::neat::c_dll
{
	VRNTZT_NEAT_C_DLL_API Neat_Evolution_Manager_Wrapper new_neat_evolution_manager(
		uint t_inputs, uint t_outputs, Neat_Evolution_Settings_Wrapper t_settings)
	{
		// create evolution manager
		Internal_Neat_Evolution_Settings settings = _convert_ev_settings(
			t_settings);
		Internal_Simplistic_Neat_Evolution_Manager* new_manager =
			new Internal_Simplistic_Neat_Evolution_Manager(t_inputs, t_outputs,
				settings);

		return _wrap_ev_manager(new_manager);
	}

	VRNTZT_NEAT_C_DLL_API void delete_neat_evolution_manager(
		Neat_Evolution_Manager_Wrapper t_wrapped_manager)
	{
		delete _unwrap_ev_manager(t_wrapped_manager);
	}

	// simple forward
	VRNTZT_NEAT_C_DLL_API void create_random_population(Neat_Evolution_Manager_Wrapper t_wrapped_manager)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapped_manager);
		manager->create_random_population();
	}

	// simple forward
	VRNTZT_NEAT_C_DLL_API void evolve_population(Neat_Evolution_Manager_Wrapper t_wrapped_manager)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapped_manager);
		manager->evolve_population();
	}

	VRNTZT_NEAT_C_DLL_API void get_population(Neat_Evolution_Manager_Wrapper t_wrapped_manager,
		Simplistic_Genotype_Wrapper* t_target, size_t t_target_size)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapped_manager);

		if (t_target_size != manager->get_population_size())
		{
			// array has wrong size
			// throw;
		}
		else
		{
			auto& population = manager->get_population();

			for (size_t i = 0; i < t_target_size; ++i)
			{
				// wrap population and store in target array
				// population contains smart pointers
				t_target[i] = _wrap_simplistic_genotype(population[i].get());
			}
		}
	}
}
