///////////////////////////////////////////////////////////////////////////////
/// \file pass_through.hpp
///
/// Definition of the pass_through transform, which is the default transform
/// of all of the expression generator metafunctions such as unary_plus<>, plus<>
/// and nary_expr<>.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_PASS_THROUGH_HPP_EAN_12_26_2006
#define BOOST_PROTO_TRANSFORM_PASS_THROUGH_HPP_EAN_12_26_2006

#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum.hpp"
#include "third_party/boost/boost/preprocessor/iteration/iterate.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/type_traits/remove_reference.hpp"
#include "third_party/boost/boost/proto/proto_fwd.hpp"
#include "third_party/boost/boost/proto/args.hpp"
#include "third_party/boost/boost/proto/transform/impl.hpp"
#include "third_party/boost/boost/proto/detail/ignore_unused.hpp"

namespace boost { namespace proto
{
    namespace detail
    {
        template<
            typename Grammar
          , typename Expr
          , typename State
          , typename Data
          , long Arity = arity_of<Expr>::value
        >
        struct pass_through_impl
        {};

        #include "third_party/boost/boost/proto/transform/detail/pass_through_impl.hpp"

        template<typename Grammar, typename Expr, typename State, typename Data>
        struct pass_through_impl<Grammar, Expr, State, Data, 0>
          : transform_impl<Expr, State, Data>
        {
            typedef Expr result_type;

            /// \param e An expression
            /// \return \c e
            /// \throw nothrow
            BOOST_FORCEINLINE
            #ifdef BOOST_PROTO_STRICT_RESULT_OF
            result_type
            #else
            typename pass_through_impl::expr_param
            #endif
            operator()(
                typename pass_through_impl::expr_param e
              , typename pass_through_impl::state_param
              , typename pass_through_impl::data_param
            ) const
            {
                return e;
            }
        };

    } // namespace detail

    /// \brief A PrimitiveTransform that transforms the child expressions
    /// of an expression node according to the corresponding children of
    /// a Grammar.
    ///
    /// Given a Grammar such as <tt>plus\<T0, T1\></tt>, an expression type
    /// that matches the grammar such as <tt>plus\<E0, E1\>::type</tt>, a
    /// state \c S and a data \c V, the result of applying the
    /// <tt>pass_through\<plus\<T0, T1\> \></tt> transform is:
    ///
    /// \code
    /// plus<
    ///     T0::result<T0(E0, S, V)>::type
    ///   , T1::result<T1(E1, S, V)>::type
    /// >::type
    /// \endcode
    ///
    /// The above demonstrates how child transforms and child expressions
    /// are applied pairwise, and how the results are reassembled into a new
    /// expression node with the same tag type as the original.
    ///
    /// The explicit use of <tt>pass_through\<\></tt> is not usually needed,
    /// since the expression generator metafunctions such as
    /// <tt>plus\<\></tt> have <tt>pass_through\<\></tt> as their default
    /// transform. So, for instance, these are equivalent:
    ///
    /// \code
    /// // Within a grammar definition, these are equivalent:
    /// when< plus<X, Y>, pass_through< plus<X, Y> > >
    /// when< plus<X, Y>, plus<X, Y> >
    /// when< plus<X, Y> > // because of when<class X, class Y=X>
    /// plus<X, Y>         // because plus<> is both a
    ///                    //   grammar and a transform
    /// \endcode
    ///
    /// For example, consider the following transform that promotes all
    /// \c float terminals in an expression to \c double.
    ///
    /// \code
    /// // This transform finds all float terminals in an expression and promotes
    /// // them to doubles.
    /// struct Promote
    ///  : or_<
    ///         when<terminal<float>, terminal<double>::type(_value) >
    ///         // terminal<>'s default transform is a no-op:
    ///       , terminal<_>
    ///         // nary_expr<> has a pass_through<> transform:
    ///       , nary_expr<_, vararg<Promote> >
    ///     >
    /// {};
    /// \endcode
    template<typename Grammar>
    struct pass_through
      : transform<pass_through<Grammar> >
    {
        template<typename Expr, typename State, typename Data>
        struct impl
          : detail::pass_through_impl<Grammar, Expr, State, Data>
        {};
    };

    /// INTERNAL ONLY
    ///
    template<typename Grammar>
    struct is_callable<pass_through<Grammar> >
      : mpl::true_
    {};

}} // namespace boost::proto

#endif
