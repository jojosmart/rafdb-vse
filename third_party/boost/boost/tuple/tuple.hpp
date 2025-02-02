//  tuple.hpp - Boost Tuple Library --------------------------------------

// Copyright (C) 1999, 2000 Jaakko Jarvi (jaakko.jarvi@cs.utu.fi)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef BOOST_TUPLE_HPP
#define BOOST_TUPLE_HPP

#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730
// Work around a compiler bug.
// boost::python::tuple has to be seen by the compiler before the
// boost::tuple class template.
namespace boost { namespace python { class tuple; }}
#endif

#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/static_assert.hpp"

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// The MSVC version
#include "third_party/boost/boost/tuple/detail/tuple_basic_no_partial_spec.hpp"

#else
// other compilers
#include "third_party/boost/boost/ref.hpp"
#include "third_party/boost/boost/tuple/detail/tuple_basic.hpp"

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace boost {    

using tuples::tuple;
using tuples::make_tuple;
using tuples::tie;
#if !defined(BOOST_NO_USING_TEMPLATE)
using tuples::get;
#elif !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
//
// The "using tuples::get" statement causes the
// Borland compiler to ICE, use forwarding
// functions instead:
//
template<int N, class HT, class TT>
inline typename tuples::access_traits<
                  typename tuples::element<N, tuples::cons<HT, TT> >::type
                >::non_const_type
get(tuples::cons<HT, TT>& c) {
  return tuples::get<N,HT,TT>(c);
} 
// get function for const cons-lists, returns a const reference to
// the element. If the element is a reference, returns the reference
// as such (that is, can return a non-const reference)
template<int N, class HT, class TT>
inline typename tuples::access_traits<
                  typename tuples::element<N, tuples::cons<HT, TT> >::type
                >::const_type
get(const tuples::cons<HT, TT>& c) {
  return tuples::get<N,HT,TT>(c);
}
#else  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//
// MSVC, using declarations don't mix with templates well,
// so use forwarding functions instead:
//
template<int N, typename Head, typename Tail>
typename tuples::detail::element_ref<N, tuples::cons<Head, Tail> >::RET
get(tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<N>* = 0)
{
   return tuples::detail::get_class<N>::get(t);
}

template<int N, typename Head, typename Tail>
typename tuples::detail::element_const_ref<N, tuples::cons<Head, Tail> >::RET
get(const tuples::cons<Head, Tail>& t, tuples::detail::workaround_holder<N>* = 0)
{
   return tuples::detail::get_class<N>::get(t);
}
#endif // BOOST_NO_USING_TEMPLATE
   
} // end namespace boost


#endif // BOOST_TUPLE_HPP
