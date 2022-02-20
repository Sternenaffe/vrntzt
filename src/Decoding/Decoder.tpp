// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// Decoder template source file

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#include <string>

#include "lib/IO/IO.hpp"

#ifndef VRNTZT_DECODER_TPP
#define VRNTZT_DECODER_TPP

namespace vrntzt::neat::decoding
{
	namespace _decoding_details
	{
		// default decoding routine with intermediate step via Generic_Genome
		template <Genotype_Type Source, Phenotype_Type Target>
		inline Target default_decode(Source& t_source)
		{
			//IO::debug("default decode from " + static_cast<std::string>(typeid(Source).name())
			//	+ " to " + static_cast<std::string>(typeid(Target).name()) + "\n");

			Generic_Genome temp = static_cast<Generic_Genome>(t_source);
			return static_cast<Target>(temp);
		}

		// template variable
		// stores function pointers to decode functions
		template <Genotype_Type Source, Phenotype_Type Target>
		Delegate<Target, Source&> decode_func =
			&default_decode<Source, Target>;
	}

	// decode Source Genome into Target via stored function pointer
	template <Genotype_Type Source, Phenotype_Type Target>
	inline Target decode(Source& t_source)
	{
		return _decoding_details::decode_func<Source, Target>(t_source);
	}

	// sets user-defined decode method for specific conversion
	template <Genotype_Type Source, Phenotype_Type Target>
	inline void set_decode_function(Delegate<Target, Source&> t_func)
	{
		_decoding_details::decode_func<Source, Target> = t_func;
	}

	// overload if function takes const Source - invalid!
	// function pointer which takes const ref is not castable to function pointer
	// which takes non-const reference
	template <Genotype_Type Source, Phenotype_Type Target>
	inline void set_decode_function(Delegate<Target, const Source&> t_func)
	{
		static_assert(false, "A 'const reference' source parameter is not allowed");
	}

	// source is no reference - invalid!
	template <Genotype_Type Source, Phenotype_Type Target>
	inline void set_decode_function(Delegate<Target, Source> t_func)
	{
		// would casting of function pointers be possible?
		// even if, object has to be taken by reference (no copying: performance &
		// internal state)
		static_assert(false, "Decode function must take a reference");
	}
}

#endif // !VRNTZT_DECODER_TPP
