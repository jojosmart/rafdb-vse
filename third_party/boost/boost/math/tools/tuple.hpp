//  (C) Copyright John Maddock 2010.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TUPLE_HPP_INCLUDED
#  define BOOST_MATH_TUPLE_HPP_INCLUDED
#  include "third_party/boost/boost/config.hpp"

#include "third_party/boost/boost/tr1/detail/config.hpp"  // for BOOST_HAS_TR1_TUPLE

#ifndef BOOST_NO_0X_HDR_TUPLE

#include <tuple>

namespace boost{ namespace math{

using ::std::tuple;

// [6.1.3.2] Tuple creation functions
using ::std::ignore;
using ::std::make_tuple;
using ::std::tie;
using ::std::get;

// [6.1.3.3] Tuple helper classes
using ::std::tuple_size;
using ::std::tuple_element;

}}

#elif defined(BOOST_HAS_TR1_TUPLE)

#include "third_party/boost/boost/tr1/tuple.hpp"

namespace boost{ namespace math{

using ::std::tr1::tuple;

// [6.1.3.2] Tuple creation functions
using ::std::tr1::ignore;
using ::std::tr1::make_tuple;
using ::std::tr1::tie;
using ::std::tr1::get;

// [6.1.3.3] Tuple helper classes
using ::std::tr1::tuple_size;
using ::std::tr1::tuple_element;

}}

#elif (defined(__BORLANDC__) && (__BORLANDC__ <= 0x600)) || (defined(_MSC_VER) && (_MSC_VER < 1310)) || defined(__IBMCPP__)

#include "third_party/boost/boost/tuple/tuple.hpp"
#include "third_party/boost/boost/tuple/tuple_comparison.hpp"
#include "third_party/boost/boost/type_traits/integral_constant.hpp"

namespace boost{ namespace math{

using ::boost::tuple;

// [6.1.3.2] Tuple creation functions
using ::boost::tuples::ignore;
using ::boost::make_tuple;
using ::boost::tie;

// [6.1.3.3] Tuple helper classes
template <class T> 
struct tuple_size 
   : public ::boost::integral_constant
   < ::std::size_t, ::boost::tuples::length<T>::value>
{};

template < int I, class T>
struct tuple_element
{
   typedef typename boost::tuples::element<I,T>::type type;
};

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
// [6.1.3.4] Element access
using ::boost::get;
#endif

} } // namespaces

#else

#include "third_party/boost/boost/fusion/include/tuple.hpp"
#include "third_party/boost/boost/fusion/include/std_pair.hpp"

namespace boost{ namespace math{

using ::boost::fusion::tuple;

// [6.1.3.2] Tuple creation functions
using ::boost::fusion::ignore;
using ::boost::fusion::make_tuple;
using ::boost::fusion::tie;
using ::boost::fusion::get;

// [6.1.3.3] Tuple helper classes
using ::boost::fusion::tuple_size;
using ::boost::fusion::tuple_element;

}}

#endif

#endif


