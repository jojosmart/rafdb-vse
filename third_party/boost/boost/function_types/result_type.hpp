
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_RESULT_TYPE_HPP_INCLUDED
#define BOOST_FT_RESULT_TYPE_HPP_INCLUDED

#include "third_party/boost/boost/blank.hpp"
#include "third_party/boost/boost/mpl/if.hpp"

#include "third_party/boost/boost/mpl/aux_/lambda_support.hpp"
#include "third_party/boost/boost/type_traits/detail/template_arity_spec.hpp"

#include "third_party/boost/boost/mpl/at.hpp"

#include "third_party/boost/boost/function_types/is_callable_builtin.hpp"
#include "third_party/boost/boost/function_types/components.hpp"

namespace boost 
{ 
  namespace function_types 
  {
    template< typename T > struct result_type;

    namespace detail
    {
      template<typename T> struct result_type_impl
        : mpl::at_c
          < typename function_types::components<T>::types, 0 >
      { };
    }

    template<typename T> struct result_type
      : mpl::if_
        < function_types::is_callable_builtin<T>
        , detail::result_type_impl<T>, boost::blank
        >::type
    { 
      BOOST_MPL_AUX_LAMBDA_SUPPORT(1,result_type,(T)) 
    };
  }
  BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,function_types::result_type)
}

#endif

