/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_ADAPTED_30122005_1420)
#define BOOST_FUSION_ADAPTED_30122005_1420

#include "third_party/boost/boost/fusion/adapted/adt.hpp"
#include "third_party/boost/boost/fusion/adapted/array.hpp"
#include "third_party/boost/boost/fusion/adapted/boost_array.hpp"
#include "third_party/boost/boost/fusion/adapted/boost_tuple.hpp"
#include "third_party/boost/boost/fusion/adapted/mpl.hpp"
#include "third_party/boost/boost/fusion/adapted/std_pair.hpp"
#include "third_party/boost/boost/fusion/adapted/struct.hpp"

// The std_tuple_iterator adaptor only supports implementations
// using variadic templates
#if !defined(BOOST_NO_VARIADIC_TEMPLATES)
#include "third_party/boost/boost/fusion/adapted/std_tuple.hpp"
#endif

#endif
