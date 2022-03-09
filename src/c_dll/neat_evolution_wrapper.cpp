// ch 2021-22
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
	Neat_Evolution_Manager_Wrapper new_neat_evolution_manager(
		const uint t_inputs, const uint t_outputs,
		const Neat_Evolution_Settings_Wrapper t_settings)
	{
		// create evolution manager
		Internal_Neat_Evolution_Settings settings = _convert_ev_settings(
			t_settings);
		Internal_Simplistic_Neat_Evolution_Manager* new_manager =
			new Internal_Simplistic_Neat_Evolution_Manager(t_inputs, t_outputs,
				settings);

		return _wrap_ev_manager(new_manager);
	}

	void delete_neat_evolution_manager(
		Neat_Evolution_Manager_Wrapper* t_wrapper)
	{
		delete _unwrap_ev_manager(*t_wrapper);
		t_wrapper->handler = nullptr;
	}

	// simple forward
	void create_random_population(const Neat_Evolution_Manager_Wrapper t_wrapper)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapper);
		manager->create_random_population();
	}

	// simple forward
	void evolve_population(Neat_Evolution_Manager_Wrapper t_wrapper)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapper);
		manager->evolve_population();
	}

	void get_population(Neat_Evolution_Manager_Wrapper t_wrapper,
		Simplistic_Genotype_Wrapper* t_target, size_t t_target_size)
	{
		Internal_Simplistic_Neat_Evolution_Manager* manager = _unwrap_ev_manager(
			t_wrapper);

		if (t_target_size != manager->get_population_size())
		{
			// array has wrong size
			// throw;
			std::cout << "wrong array size!\n";
		}
		else
		{
			auto& population = manager->get_population();

			for (size_t i = 0; i < t_target_size; ++i)
			{
				// wrap population and store in target array
				// population contains smart pointers
				Internal_Simplistic_Genotype* genotype_ptr =
					population[i].get();
				t_target[i] = _wrap_simplistic_genotype(genotype_ptr);
			}
		}
	}
}
