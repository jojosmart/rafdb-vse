
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_IS_MEMBER_OBJECT_POINTER_HPP_INCLUDED
#define BOOST_FT_IS_MEMBER_OBJECT_POINTER_HPP_INCLUDED

#include "third_party/boost/boost/mpl/aux_/lambda_support.hpp"
#include "third_party/boost/boost/type_traits/detail/template_arity_spec.hpp"

#include "third_party/boost/boost/function_types/components.hpp"

namespace boost 
{ 
  namespace function_types 
  {
    template< typename T > 
    struct is_member_object_pointer
      : function_types::detail::represents_impl
        < function_types::components<T>
        , detail::member_object_pointer_tag >
    { 
      BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_member_object_pointer,(T))
    };
  }
  BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,function_types::is_member_object_pointer)
} 

#endif 

