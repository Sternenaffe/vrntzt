// ch 2021
// c++20
// v1.0.0
// vrntzt
// NEAT header
// concept definitions

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * 
  */

#ifndef VRNTZT_CONCEPTS_HPP
#define VRNTZT_CONCEPTS_HPP

#include <iostream>

namespace vrntzt::neat
{
	// forward dec
	template <typename T>
	class IGenome;

	template <typename T>
	class IPhenome;

	// concepts for controlling Target and Source of decode functions
	template <class T>
	concept Genome_Type = std::is_base_of<IGenome<T>, T>::value;

	template <class T>
	concept Phenome_Type = std::is_base_of<IPhenome<T>, T>::value;
}

#endif // !VRNTZT_CONCEPTS_HPP
