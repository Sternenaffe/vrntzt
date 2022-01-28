// ch 2021
// c++14
// v1.0.0
// miscellaneous utility
// interface extension
ERROR - NOT USED

/* CHANGELOG
 *
 */

 /* SUMMARY
  *
  * interface extension header
  * 
  * 
  */

#ifndef EMPTY_HEADER_HPP
#define EMPTY_HEADER_HPP

#define INTERFACE(INTERFACE_CLASS, THIS_CLASS) \
    INTERFACE_CLASS<THIS_CLASS> _interface_unused; \
    friend INTERFACE_CLASS<THIS_CLASS>

#endif // !EMPTY_HEADER_HPP
