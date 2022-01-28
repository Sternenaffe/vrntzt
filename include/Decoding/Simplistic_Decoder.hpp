// ch 2021-22
// c++20
// v1.0.0
// vrntzt
// neat
// decoder extension header

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * adds a decode spezialization for simplistic genome to simplistic
  * phenome
  */

#ifndef VRNTZT_SIMPLISTIC_DECODER_HPP
#define VRNTZT_SIMPLISTIC_DECODER_HPP

#include "Decoder.hpp"
#include "include/Genomes/Simplistic_Genome.hpp"
#include "include/Phenomes/Simplistic_Phenome.hpp"

namespace vrntzt::neat::decoding
{
	// adds decode functions to decoder
	void init_simplistic_decode();

	Simplistic_Phenome decode_simplistic_genome(Simplistic_Genome& t_genome);
}

#endif // !VRNTZT_SIMPLISTIC_DECODER_HPP
