
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_FUNCTION_REFERENCE_HPP_INCLUDED
#define BOOST_FT_FUNCTION_REFERENCE_HPP_INCLUDED

#include "third_party/boost/boost/mpl/aux_/lambda_support.hpp"
#include "third_party/boost/boost/type_traits/detail/template_arity_spec.hpp"

#include "third_party/boost/boost/function_types/function_type.hpp"

namespace boost 
{ 
  namespace function_types 
  {
    template<typename Types, typename Tag = null_tag> struct function_reference
    {
      typedef typename function_types::function_type<Types,Tag>::type & type;

      BOOST_MPL_AUX_LAMBDA_SUPPORT(2,function_reference,(Types,Tag))
    };
  } 
  BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(2,function_types::function_reference)
} 

#endif

