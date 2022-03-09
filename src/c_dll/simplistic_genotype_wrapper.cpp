// ch 2021-22
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat evolution wrapping header

#include "include/vrntzt_pch.h"
#define VRNTZT_NEAT_C_DLL_EXPORT
#include "include/c_dll/simplistic_genotype_wrapper.h"

#include "lib/utility/include/trivial_typedefs.hpp"

#include "include/c_dll/internal_alias.hpp"
#include "include/c_dll/wrapping_helper.hpp"
#include "include/Genotype/Simplistic_Genotype.hpp"
#include "include/Decoding/Simplistic_Decoder.hpp"

namespace vrntzt::neat::c_dll
{
	constexpr auto& _decode = vrntzt::neat::decoding::decode_Simplistic_Genotype;

	// simple forward
	float get_fitness(const Simplistic_Genotype t_wrapper)
	{
		Internal_Simplistic_Genotype* genotype = _unwrap_simplistic_genotype(t_wrapper);
		return genotype->get_fitness();
	}

	// simple forward
	void set_fitness(const Simplistic_Genotype t_wrapper, const float t_fitness)
	{
		Internal_Simplistic_Genotype* genotype = _unwrap_simplistic_genotype(t_wrapper);
		genotype->set_fitness(t_fitness);
	}

	Simplistic_Phenotype decode(const Simplistic_Genotype t_wrapper)
	{
		Internal_Simplistic_Genotype* genotype = _unwrap_simplistic_genotype(t_wrapper);

		Internal_Simplistic_Phenotype tmp_genotype = _decode(*genotype);
		// reserve memory - needs to free!
		Internal_Simplistic_Phenotype* new_phenotype =
			new Internal_Simplistic_Phenotype(std::move(tmp_genotype));

		return _wrap_simplistic_phenotype(new_phenotype);;
	}
}

