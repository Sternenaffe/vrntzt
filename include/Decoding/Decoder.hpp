// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// Decoder

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * handles decoding of a arbitrary genome into phenome
  * core is a template variable which stores function pointers to (user-defined)
  * decode functions
  * 
  * provides two functions:
  * 
  * Target decode<Target, Source>(Source&)
  * 
  * void set_decode_function<Target, Source>(Delegate<Target, Source&> t_func)
  * 
  * how should decode functions be implemented?
  * 
  * decode functions should be implemented as static functions in a struct/class
  * which inherits from the relevant genome, so that it can access all protected
  * members
  */

// TO DO: find way to create multiple instances (enables different decode 
//		  functions for same conversion)

#ifndef VRNTZT_DECODER_HPP
#define VRNTZT_DECODER_HPP

#include "lib/utility/include/various.hpp"

#include "src/vrntzt_concepts.hpp"
#include "include/Genomes/IGenotype.hpp"
#include "include/Phenomes/IPhenotype.hpp"
#include "include/Generic_Genome.hpp"

namespace vrntzt::neat::decoding
{
	using u_lib::Delegate;

	// decode Source Genome into Target via stored function pointer
	template <Genome_Type Source, Phenome_Type Target>
	inline Target decode(Source& t_source);

	// sets user-defined decode method for specific conversion
	template <Genome_Type Source, Phenome_Type Target>
	inline void set_decode_function(Delegate<Target, Source&> t_func);

	// overload if function takes const Source - invalid!
	// function pointer which takes const ref is not castable to function pointer
	// which takes non-const reference
	template <Genome_Type Source, Phenome_Type Target>
	inline void set_decode_function(Delegate<Target, const Source&> t_func);

	// source is no reference - invalid!
	template <Genome_Type Source, Phenome_Type Target>
	inline void set_decode_function(Delegate<Target, Source> t_func);
}

#include "src/Decoding/Decoder.tpp"

#endif // !VRNTZT_DECODER_HPP
