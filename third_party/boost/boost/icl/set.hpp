/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2011: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_SET_HPP_JOFA_070519
#define BOOST_ICL_SET_HPP_JOFA_070519

#include "third_party/boost/boost/icl/impl_config.hpp"

#if defined(ICL_USE_BOOST_MOVE_IMPLEMENTATION)
#   include "third_party/boost/boost/container/set.hpp"
#elif defined(ICL_USE_STD_IMPLEMENTATION)
#   include <set>
#else 
#   include <set>
#endif

#include "third_party/boost/boost/icl/concept/associative_element_container.hpp"


}} // namespace icl boost

#endif // BOOST_ICL_SET_HPP_JOFA_070519

