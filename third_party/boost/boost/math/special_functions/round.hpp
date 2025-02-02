//  Copyright John Maddock 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ROUND_HPP
#define BOOST_MATH_ROUND_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "third_party/boost/boost/math/tools/config.hpp"
#include "third_party/boost/boost/math/policies/error_handling.hpp"
#include "third_party/boost/boost/math/special_functions/fpclassify.hpp"

namespace boost{ namespace math{

template <class T, class Policy>
inline T round(const T& v, const Policy& pol)
{
   BOOST_MATH_STD_USING
   if(!(boost::math::isfinite)(v))
      return policies::raise_rounding_error("boost::math::round<%1%>(%1%)", 0, v, v, pol);
   return v < 0 ? static_cast<T>(ceil(v - 0.5f)) : static_cast<T>(floor(v + 0.5f));
}
template <class T>
inline T round(const T& v)
{
   return round(v, policies::policy<>());
}
//
// The following functions will not compile unless T has an
// implicit convertion to the integer types.  For user-defined
// number types this will likely not be the case.  In that case
// these functions should either be specialized for the UDT in
// question, or else overloads should be placed in the same 
// namespace as the UDT: these will then be found via argument
// dependent lookup.  See our concept archetypes for examples.
//
template <class T, class Policy>
inline int iround(const T& v, const Policy& pol)
{
   BOOST_MATH_STD_USING
   T r = boost::math::round(v, pol);
   if((r > (std::numeric_limits<int>::max)()) || (r < (std::numeric_limits<int>::min)()))
      return static_cast<int>(policies::raise_rounding_error("boost::math::iround<%1%>(%1%)", 0, v, 0, pol));
   return static_cast<int>(r);
}
template <class T>
inline int iround(const T& v)
{
   return iround(v, policies::policy<>());
}

template <class T, class Policy>
inline long lround(const T& v, const Policy& pol)
{
   BOOST_MATH_STD_USING
   T r = boost::math::round(v, pol);
   if((r > (std::numeric_limits<long>::max)()) || (r < (std::numeric_limits<long>::min)()))
      return static_cast<long int>(policies::raise_rounding_error("boost::math::lround<%1%>(%1%)", 0, v, 0L, pol));
   return static_cast<long int>(r);
}
template <class T>
inline long lround(const T& v)
{
   return lround(v, policies::policy<>());
}

#ifdef BOOST_HAS_LONG_LONG

template <class T, class Policy>
inline boost::long_long_type llround(const T& v, const Policy& pol)
{
   BOOST_MATH_STD_USING
   T r = boost::math::round(v, pol);
   if((r > (std::numeric_limits<boost::long_long_type>::max)()) || (r < (std::numeric_limits<boost::long_long_type>::min)()))
      return static_cast<boost::long_long_type>(policies::raise_rounding_error("boost::math::llround<%1%>(%1%)", 0, v, 0LL, pol));
   return static_cast<boost::long_long_type>(r);
}
template <class T>
inline boost::long_long_type llround(const T& v)
{
   return llround(v, policies::policy<>());
}

#endif

}} // namespaces

#endif // BOOST_MATH_ROUND_HPP
