/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING
#ifndef PHOENIX_OBJECT_DETAIL_CONSTRUCT_EVAL_HPP
#define PHOENIX_OBJECT_DETAIL_CONSTRUCT_EVAL_HPP

#include "third_party/boost/boost/preprocessor/iterate.hpp"
#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"

#define PHOENIX_EVAL_ARG(z, n, data)                                            \
    BOOST_PP_CAT(_, n).eval(env)

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_COMPOSITE_LIMIT,                                            \
    "third_party/boost/boost/spirit/home/phoenix/object/detail/construct_eval.hpp"))
#include BOOST_PP_ITERATE()

#undef PHOENIX_EVAL_ARG
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename RT, typename Env, BOOST_PP_ENUM_PARAMS(N, typename A)>
    static RT
    eval(Env const& env, BOOST_PP_ENUM_BINARY_PARAMS(N, A, & _))
    {
        return RT(BOOST_PP_ENUM(N, PHOENIX_EVAL_ARG, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


