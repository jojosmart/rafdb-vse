//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_DEREFERENCE_HPP_INCLUDED
#define BOOST_TT_HAS_DEREFERENCE_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_dereference
#define BOOST_TT_TRAIT_OP *
#define BOOST_TT_FORBIDDEN_IF\
   /* void* or fundamental */\
   ::boost::type_traits::ice_or<\
      ::boost::type_traits::ice_and<\
         ::boost::is_pointer< Rhs_noref >::value,\
         ::boost::is_void< Rhs_noptr >::value\
      >::value,\
      ::boost::is_fundamental< Rhs_nocv >::value\
   >::value


#include "third_party/boost/boost/type_traits/detail/has_prefix_operator.hpp"

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
