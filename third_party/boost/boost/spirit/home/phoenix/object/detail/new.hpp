/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING
#ifndef PHOENIX_OBJECT_DETAIL_NEW_HPP
#define PHOENIX_OBJECT_DETAIL_NEW_HPP

#include "third_party/boost/boost/preprocessor/iterate.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_COMPOSITE_LIMIT,                                            \
    "third_party/boost/boost/spirit/home/phoenix/object/detail/new.hpp"))
#include BOOST_PP_ITERATE()

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename T, BOOST_PP_ENUM_PARAMS(N, typename A)>
    inline actor<typename as_composite<detail::new_eval<T>
        , BOOST_PP_ENUM_PARAMS(N, A)>::type>
    new_(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& _))
    {
        return compose<detail::new_eval<T> >(BOOST_PP_ENUM_PARAMS(N, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


