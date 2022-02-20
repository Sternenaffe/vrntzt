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
	class IGenotype;

	template <typename T>
	class IPhenotype;

	// concepts for controlling Target and Source of decode functions
	template <class T>
	concept Genotype_Type = std::is_base_of<IGenotype<T>, T>::value;

	template <class T>
	concept Phenotype_Type = std::is_base_of<IPhenotype<T>, T>::value;
}

#endif // !VRNTZT_CONCEPTS_HPP
