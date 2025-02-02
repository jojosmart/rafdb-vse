
#if !BOOST_PHOENIX_IS_ITERATING

#ifndef BOOST_PHOENIX_OPERATOR_MEMBER_HPP
#define BOOST_PHOENIX_OPERATOR_MEMBER_HPP

#include "third_party/boost/boost/phoenix/core/limits.hpp"
#include "third_party/boost/boost/get_pointer.hpp"
#include "third_party/boost/boost/phoenix/core/domain.hpp"
#include "third_party/boost/boost/phoenix/core/meta_grammar.hpp"
#include "third_party/boost/boost/phoenix/core/call.hpp"
#include "third_party/boost/boost/phoenix/operator/detail/mem_fun_ptr_gen.hpp"
#include "third_party/boost/boost/phoenix/support/iterate.hpp"
#include "third_party/boost/boost/type_traits/is_member_function_pointer.hpp"
#include "third_party/boost/boost/proto/operators.hpp"

#if !defined(BOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES)

#include "third_party/boost/boost/phoenix/operator/preprocessed/member.hpp"

#else

#if defined(__WAVE__) && defined(BOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/member_" BOOST_PHOENIX_LIMIT_STR ".hpp")
#endif

/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if defined(__WAVE__) && defined(BOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#include "third_party/boost/boost/phoenix/operator/detail/define_operator.hpp"

namespace boost { namespace phoenix
{
    BOOST_PHOENIX_BINARY_OPERATORS((mem_ptr))

    template <typename Object, typename MemPtr>
    inline
    typename boost::enable_if<
        is_member_function_pointer<MemPtr>
      , detail::mem_fun_ptr_gen<actor<Object>, MemPtr> const
    >::type
    operator->*(actor<Object> const& obj, MemPtr ptr)
    {
        return detail::mem_fun_ptr_gen<actor<Object>, MemPtr>(obj, ptr);
    }

    using proto::exprns_::operator->*;

    namespace result_of
    {
        template <
            typename Context
          , BOOST_PHOENIX_typename_A_void(BOOST_PHOENIX_LIMIT)
          , typename Dummy = void
        >
        struct mem_fun_ptr_eval;

        #include "third_party/boost/boost/phoenix/operator/detail/mem_fun_ptr_eval_result_of.hpp"
    }

    struct mem_fun_ptr_eval
    {
        template<typename Sig>
        struct result;

    #define BOOST_PHOENIX_MEMBER_EVAL(Z, N, D)                                  \
        BOOST_PP_COMMA_IF(BOOST_PP_NOT(BOOST_PP_EQUAL(N, 2)))                   \
        boost::phoenix::eval(BOOST_PP_CAT(a, N), ctx)                           \
    /**/

    #define BOOST_PHOENIX_ITERATION_PARAMS                                      \
        (3, (2, BOOST_PHOENIX_LIMIT,                                            \
        "third_party/boost/boost/phoenix/operator/member.hpp"))                                   \
    /**/
        #include BOOST_PHOENIX_ITERATE()
    #undef BOOST_PHOENIX_MEMBER_EVAL
    };

    template <typename Dummy>
    struct default_actions::when<rule::mem_fun_ptr, Dummy>
        : call<mem_fun_ptr_eval>
    {};
}}

#include "third_party/boost/boost/phoenix/operator/detail/undef_operator.hpp"

#if defined(__WAVE__) && defined(BOOST_PHOENIX_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // BOOST_PHOENIX_DONT_USE_PREPROCESSED_FILES

#endif

#else // BOOST_PHOENIX_IS_ITERATING

        template <typename This, BOOST_PHOENIX_typename_A, typename Context>
        struct result<This(BOOST_PHOENIX_A, Context)>
            : result<This(BOOST_PHOENIX_A_const_ref, Context)>
        {};

        template <typename This, BOOST_PHOENIX_typename_A, typename Context>
        struct result<This(BOOST_PHOENIX_A_ref, Context)>
            : result_of::mem_fun_ptr_eval<Context, BOOST_PHOENIX_A>
        {};

        template <BOOST_PHOENIX_typename_A, typename Context>
        typename result_of::mem_fun_ptr_eval<Context, BOOST_PHOENIX_A>::type
        operator()(
            BOOST_PHOENIX_A_const_ref_a
          , Context & ctx
        ) const
        {
            return
                (
                    get_pointer(boost::phoenix::eval(a0, ctx))
                    ->*boost::phoenix::eval(a1, ctx)
                )(
                    BOOST_PP_REPEAT_FROM_TO(
                        2
                      , BOOST_PHOENIX_ITERATION
                      , BOOST_PHOENIX_MEMBER_EVAL
                      , BOOST_PHOENIX_ITERATION
                    )
                );
        }
        

#endif
