/*=============================================================================
    Copyright (c) 2005-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING
#if !defined(FUSION_AS_DEQUE_20061213_2210)
#define FUSION_AS_DEQUE_20061213_2210

#include "third_party/boost/boost/preprocessor/iterate.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat.hpp"
#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/inc.hpp"
#include "third_party/boost/boost/preprocessor/dec.hpp"
#include "third_party/boost/boost/fusion/container/deque/deque.hpp"
#include "third_party/boost/boost/fusion/iterator/value_of.hpp"
#include "third_party/boost/boost/fusion/iterator/deref.hpp"
#include "third_party/boost/boost/fusion/iterator/next.hpp"

namespace boost { namespace fusion { namespace detail
{
    template <int size>
    struct as_deque;

    template <>
    struct as_deque<0>
    {
        template <typename Iterator>
        struct apply
        {
            typedef deque<> type;
        };

        template <typename Iterator>
        static typename apply<Iterator>::type
        call(Iterator)
        {
            return deque<>();
        }
    };
}}}

#if !defined(BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include "third_party/boost/boost/fusion/container/deque/detail/preprocessed/as_deque.hpp"
#else
#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/as_deque" FUSION_MAX_DEQUE_SIZE_STR ".hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace boost { namespace fusion { namespace detail
{
#define BOOST_FUSION_NEXT_ITERATOR(z, n, data)                                  \
    typedef typename fusion::result_of::next<BOOST_PP_CAT(I, n)>::type          \
        BOOST_PP_CAT(I, BOOST_PP_INC(n));

#define BOOST_FUSION_NEXT_CALL_ITERATOR(z, n, data)                             \
    typename gen::BOOST_PP_CAT(I, BOOST_PP_INC(n))                              \
        BOOST_PP_CAT(i, BOOST_PP_INC(n)) = fusion::next(BOOST_PP_CAT(i, n));

#define BOOST_FUSION_VALUE_OF_ITERATOR(z, n, data)                              \
    typedef typename fusion::result_of::value_of<BOOST_PP_CAT(I, n)>::type      \
        BOOST_PP_CAT(T, n);

#define BOOST_PP_FILENAME_1 "third_party/boost/boost/fusion/container/deque/detail/as_deque.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_DEQUE_SIZE)
#include BOOST_PP_ITERATE()

#undef BOOST_FUSION_NEXT_ITERATOR
#undef BOOST_FUSION_NEXT_CALL_ITERATOR
#undef BOOST_FUSION_VALUE_OF_ITERATOR

}}}

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define N BOOST_PP_ITERATION()

    template <>
    struct as_deque<N>
    {
        template <typename I0>
        struct apply
        {
            BOOST_PP_REPEAT(N, BOOST_FUSION_NEXT_ITERATOR, _)
            BOOST_PP_REPEAT(N, BOOST_FUSION_VALUE_OF_ITERATOR, _)
            typedef deque<BOOST_PP_ENUM_PARAMS(N, T)> type;
        };

        template <typename Iterator>
        static typename apply<Iterator>::type
        call(Iterator const& i0)
        {
            typedef apply<Iterator> gen;
            typedef typename gen::type result;
            BOOST_PP_REPEAT(BOOST_PP_DEC(N), BOOST_FUSION_NEXT_CALL_ITERATOR, _)
            return result(BOOST_PP_ENUM_PARAMS(N, *i));
        }
    };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)

