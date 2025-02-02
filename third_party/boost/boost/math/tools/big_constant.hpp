
//  Copyright (c) 2011 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_BIG_CONSTANT_HPP
#define BOOST_MATH_TOOLS_BIG_CONSTANT_HPP

#include "third_party/boost/boost/math/tools/config.hpp"
#include "third_party/boost/boost/lexical_cast.hpp"
#include "third_party/boost/boost/type_traits/is_convertible.hpp"

namespace boost{ namespace math{ namespace tools{

template <class T>
inline T make_big_value(long double v, const char*, mpl::true_ const&, mpl::false_ const&)
{
   return static_cast<T>(v);
}
template <class T>
inline T make_big_value(long double v, const char*, mpl::true_ const&, mpl::true_ const&)
{
   return static_cast<T>(v);
}
template <class T>
inline T make_big_value(long double, const char* s, mpl::false_ const&, mpl::false_ const&)
{
   return boost::lexical_cast<T>(s);
}
template <class T>
inline const char* make_big_value(long double, const char* s, mpl::false_ const&, mpl::true_ const&)
{
   return s;
}

//
// For constants which might fit in a long double (if it's big enough):
//
#define BOOST_MATH_BIG_CONSTANT(T, D, x)\
   boost::math::tools::make_big_value<T>(BOOST_JOIN(x, L), BOOST_STRINGIZE(x), mpl::bool_<D <= std::numeric_limits<long double>::digits>(), boost::is_convertible<const char*, T>())
//
// For constants too huge for any conceivable long double (and which generate compiler errors if we try and declare them as such):
//
#define BOOST_MATH_HUGE_CONSTANT(T, D, x)\
   boost::math::tools::make_big_value<T>(0.0L, BOOST_STRINGIZE(x), mpl::bool_<false>(), boost::is_convertible<const char*, T>())

}}} // namespaces

#endif

