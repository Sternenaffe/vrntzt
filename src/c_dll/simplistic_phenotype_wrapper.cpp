// ch 2021-22
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat evolution wrapping header

#include "include/vrntzt_pch.h"
#define VRNTZT_NEAT_C_DLL_EXPORT
#include "include/c_dll/simplistic_phenotype_wrapper.h"

#include "lib/utility/include/trivial_typedefs.hpp"

#include "include/c_dll/internal_alias.hpp"
#include "include/c_dll/wrapping_helper.hpp"
#include "include/Phenotype/Simplistic_Phenotype.hpp"

namespace vrntzt::neat::c_dll
{
	void delete_simplistic_phenotype(Simplistic_Phenotype_Wrapper* t_wrapper)
	{
		delete _unwrap_simplistic_phenotype(*t_wrapper);
		t_wrapper->handler = nullptr;
	}

	void set_input(const Simplistic_Phenotype_Wrapper t_wrapper,
		const size_t t_index, const float t_value)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		simplistic_phenotype->set_input(t_index, t_value);
	}

	float get_input(const Simplistic_Phenotype_Wrapper t_wrapper,
		const size_t t_index)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		return simplistic_phenotype->get_input(t_index);
	}

	float get_output(const Simplistic_Phenotype_Wrapper t_wrapper,
		const size_t t_index)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		return simplistic_phenotype->get_output(t_index);
	}

	void activate(const Simplistic_Phenotype_Wrapper t_wrapper, uint t_iterations)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		simplistic_phenotype->activate(t_iterations);
	}

	bool is_working(const Simplistic_Phenotype_Wrapper t_wrapper)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		return simplistic_phenotype->is_working();
	}

	void reset(const Simplistic_Phenotype_Wrapper t_wrapper)
	{
		Internal_Simplistic_Phenotype* simplistic_phenotype =
			_unwrap_simplistic_phenotype(t_wrapper);
		simplistic_phenotype->reset();
	}
}

