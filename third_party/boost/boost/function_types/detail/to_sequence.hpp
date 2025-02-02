
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED
#define BOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED

#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"
#include "third_party/boost/boost/mpl/is_sequence.hpp"
#include "third_party/boost/boost/mpl/placeholders.hpp"
#include "third_party/boost/boost/type_traits/add_reference.hpp"

#include "third_party/boost/boost/function_types/is_callable_builtin.hpp"

namespace boost { namespace function_types { namespace detail {

// wrap first arguments in components, if callable builtin type
template<typename T>
struct to_sequence
{
  typedef typename
   mpl::eval_if
   < is_callable_builtin<T>
   , to_sequence< components<T> >
   , mpl::identity< T >
   >::type
  type;
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// reduce template instantiations, if possible
template<typename T, typename U>
struct to_sequence< components<T,U> > 
{
  typedef typename components<T,U>::types type;
};
#endif

} } } // namespace ::boost::function_types::detail

#endif

