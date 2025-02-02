//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_LOGICAL_NOT_HPP_INCLUDED
#define BOOST_TT_HAS_LOGICAL_NOT_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_logical_not
#define BOOST_TT_TRAIT_OP !
#define BOOST_TT_FORBIDDEN_IF\
   false

#include "third_party/boost/boost/type_traits/detail/has_prefix_operator.hpp"

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
