//  Copyright (c) 2001-2011 Joel de Guzman
//  Copyright (c) 2001-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING

#include "third_party/boost/boost/preprocessor/iterate.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"

#define BOOST_PP_FILENAME_1 \
    "third_party/boost/boost/spirit/home/karma/nonterminal/detail/fcall.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, SPIRIT_ARGUMENTS_LIMIT)
#include BOOST_PP_ITERATE()

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <BOOST_PP_ENUM_PARAMS(N, typename A)>
    typename lazy_enable_if_c<
        (params_size == N)
      , proto::terminal<
            spirit::karma::parameterized_nonterminal<
                parameterized_subject_type
              , fusion::vector<BOOST_PP_ENUM_PARAMS(N, A)> >
        >
    >::type
    operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& f)) const
    {
        typedef fusion::vector<BOOST_PP_ENUM_PARAMS(N, A)> vector_type;
        typedef spirit::karma::parameterized_nonterminal<
            parameterized_subject_type, vector_type> parameterized_type;
        typedef typename proto::terminal<parameterized_type>::type result_type;

        return result_type::make(
            parameterized_type(
                this->get_parameterized_subject()
              , fusion::make_vector(BOOST_PP_ENUM_PARAMS(N, f)))
        );
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


