// ch 2021-22
// c
// v1.0.0
// vrntzt
// neat
// c-dll neat simplistic phenotype wrapping header

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  *
  */

  // TODO: use type erasure in order to store managers with different genotypes 
  //		 or phenotypes

#ifndef VRNTZT_NEAT_C_DLL_SIMPLISTIC_PHENOTYPE_WRAPPER_H
#define VRNTZT_NEAT_C_DLL_SIMPLISTIC_PHENOTYPE_WRAPPER_H

#ifdef VRNTZT_NEAT_C_DLL_EXPORT
#define VRNTZT_NEAT_C_DLL_API __declspec(dllexport)
#else
#define VRNTZT_NEAT_C_DLL_API __declspec(dllimport)
#endif // VRNTZT_NEAT_C_DLL_EXPORT

namespace vrntzt::neat::c_dll
{
	typedef unsigned int uint;

	// struct is only for type safety
	extern "C"
	struct Simplistic_Phenotype
	{
		void* handler;
	};

	// important function
	// delete existing Neat_Evolution_Manager and invalidates all corresponding
	// wrappers
	extern "C" VRNTZT_NEAT_C_DLL_API
	void delete_simplistic_phenotype(Simplistic_Phenotype* t_wrapper);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void set_input(const Simplistic_Phenotype t_wrapper, const size_t t_index,
		const float t_value);
	
	extern "C" VRNTZT_NEAT_C_DLL_API
	float get_input(const Simplistic_Phenotype t_wrapper, const size_t t_index);

	extern "C" VRNTZT_NEAT_C_DLL_API
	float get_output(const Simplistic_Phenotype t_wrapper, const size_t t_index);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void activate(const Simplistic_Phenotype t_wrapper, uint t_iterations);

	extern "C" VRNTZT_NEAT_C_DLL_API
	bool is_working(const Simplistic_Phenotype t_wrapper);

	extern "C" VRNTZT_NEAT_C_DLL_API
	void reset(const Simplistic_Phenotype t_wrapper);
}

#endif // !VRNTZT_NEAT_C_DLL_SIMPLISTIC_PHENOTYPE_WRAPPER_H
