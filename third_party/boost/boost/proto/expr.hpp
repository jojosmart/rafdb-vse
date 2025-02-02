///////////////////////////////////////////////////////////////////////////////
/// \file expr.hpp
/// Contains definition of expr\<\> class template.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_EXPR_HPP_EAN_04_01_2005
#define BOOST_PROTO_EXPR_HPP_EAN_04_01_2005

#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/arithmetic/dec.hpp"
#include "third_party/boost/boost/preprocessor/selection/max.hpp"
#include "third_party/boost/boost/preprocessor/iteration/iterate.hpp"
#include "third_party/boost/boost/preprocessor/facilities/intercept.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_binary_params.hpp"
#include "third_party/boost/boost/utility/addressof.hpp"
#include "third_party/boost/boost/proto/proto_fwd.hpp"
#include "third_party/boost/boost/proto/args.hpp"
#include "third_party/boost/boost/proto/traits.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(push)
# pragma warning(disable : 4510) // default constructor could not be generated
# pragma warning(disable : 4512) // assignment operator could not be generated
# pragma warning(disable : 4610) // user defined constructor required
#endif

namespace boost { namespace proto
{

    namespace detail
    {
        struct not_a_valid_type
        {
        private:
            not_a_valid_type()
            {}
        };
        
        template<typename Tag, typename Arg>
        struct address_of_hack
        {
            typedef not_a_valid_type type;
        };

        template<typename Expr>
        struct address_of_hack<proto::tag::address_of, Expr &>
        {
            typedef Expr *type;
        };

        template<typename T, typename Expr, typename Arg0>
        BOOST_FORCEINLINE
        Expr make_terminal(T &t, Expr *, proto::term<Arg0> *)
        {
            Expr that = {t};
            return that;
        }

        template<typename T, typename Expr, typename Arg0, std::size_t N>
        BOOST_FORCEINLINE
        Expr make_terminal(T (&t)[N], Expr *, proto::term<Arg0[N]> *)
        {
            Expr that;
            for(std::size_t i = 0; i < N; ++i)
            {
                that.child0[i] = t[i];
            }
            return that;
        }

        template<typename T, typename Expr, typename Arg0, std::size_t N>
        BOOST_FORCEINLINE
        Expr make_terminal(T const(&t)[N], Expr *, proto::term<Arg0[N]> *)
        {
            Expr that;
            for(std::size_t i = 0; i < N; ++i)
            {
                that.child0[i] = t[i];
            }
            return that;
        }

        template<typename T, typename U>
        struct same_cv
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct same_cv<T const, U>
        {
            typedef U const type;
        };
    }

    namespace result_of
    {
        /// \brief A helper metafunction for computing the
        /// return type of \c proto::expr\<\>::operator().
        template<typename Sig, typename This, typename Domain>
        struct funop;

        #include "third_party/boost/boost/proto/detail/funop.hpp"
    }

    namespace exprns_
    {
        // This is where the basic_expr specializations are
        // actually defined:
        #include "third_party/boost/boost/proto/detail/basic_expr.hpp"

        // This is where the expr specialization are
        // actually defined:
        #include "third_party/boost/boost/proto/detail/expr.hpp"
    }

    /// \brief Lets you inherit the interface of an expression
    /// while hiding from Proto the fact that the type is a Proto
    /// expression.
    template<typename Expr>
    struct unexpr
      : Expr
    {
        BOOST_PROTO_UNEXPR()

        BOOST_FORCEINLINE
        explicit unexpr(Expr const &e)
          : Expr(e)
        {}
        
        using Expr::operator =;
    };

}}

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma warning(pop)
#endif

#endif // BOOST_PROTO_EXPR_HPP_EAN_04_01_2005
