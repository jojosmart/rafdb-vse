/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING
#if !defined(FUSION_MAKE_DEQUE_07162005_0243)
#define FUSION_MAKE_DEQUE_07162005_0243

#include "third_party/boost/boost/preprocessor/iterate.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params_with_a_default.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"
#include "third_party/boost/boost/fusion/container/deque/deque.hpp"
#include "third_party/boost/boost/fusion/support/detail/as_fusion_element.hpp"

#if !defined(BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include "third_party/boost/boost/fusion/container/generation/detail/preprocessed/make_deque.hpp"
#else
#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/make_deque" FUSION_MAX_DEQUE_SIZE_STR".hpp")
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

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template <
            BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
                FUSION_MAX_DEQUE_SIZE, typename T, void_)
          , typename Extra = void_
        >
        struct make_deque;

        template <>
        struct make_deque<>
        {
            typedef deque<> type;
        };
    }

    inline deque<>
    make_deque()
    {
        return deque<>();
    }

#define BOOST_FUSION_AS_FUSION_ELEMENT(z, n, data)                               \
    typename detail::as_fusion_element<BOOST_PP_CAT(T, n)>::type

#define BOOST_PP_FILENAME_1 "third_party/boost/boost/fusion/container/generation/make_deque.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_DEQUE_SIZE)
#include BOOST_PP_ITERATE()

#undef BOOST_FUSION_AS_FUSION_ELEMENT

}}

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

    namespace result_of
    {
        template <BOOST_PP_ENUM_PARAMS(N, typename T)>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        #define TEXT(z, n, text) , text
        struct make_deque< BOOST_PP_ENUM_PARAMS(N, T) BOOST_PP_REPEAT_FROM_TO(BOOST_PP_DEC(N), FUSION_MAX_DEQUE_SIZE, TEXT, void_) >
        #undef TEXT
#else
        struct make_deque<BOOST_PP_ENUM_PARAMS(N, T)>
#endif
        {
            typedef deque<BOOST_PP_ENUM(N, BOOST_FUSION_AS_FUSION_ELEMENT, _)> type;
        };
    }

    template <BOOST_PP_ENUM_PARAMS(N, typename T)>
    inline deque<BOOST_PP_ENUM(N, BOOST_FUSION_AS_FUSION_ELEMENT, _)>
    make_deque(BOOST_PP_ENUM_BINARY_PARAMS(N, T, const& _))
    {
        return deque<BOOST_PP_ENUM(N, BOOST_FUSION_AS_FUSION_ELEMENT, _)>(
            BOOST_PP_ENUM_PARAMS(N, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)

