///////////////////////////////////////////////////////////////////////////////
/// \file make.hpp
/// Contains definition of the make<> transform.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_MAKE_HPP_EAN_12_02_2007
#define BOOST_PROTO_TRANSFORM_MAKE_HPP_EAN_12_02_2007

#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params_with_a_default.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"
#include "third_party/boost/boost/preprocessor/facilities/intercept.hpp"
#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/iteration/iterate.hpp"
#include "third_party/boost/boost/preprocessor/selection/max.hpp"
#include "third_party/boost/boost/preprocessor/arithmetic/inc.hpp"
#include "third_party/boost/boost/mpl/and.hpp"
#include "third_party/boost/boost/mpl/aux_/has_type.hpp"
#include "third_party/boost/boost/proto/detail/template_arity.hpp"
#include "third_party/boost/boost/utility/result_of.hpp"
#include "third_party/boost/boost/proto/proto_fwd.hpp"
#include "third_party/boost/boost/proto/traits.hpp"
#include "third_party/boost/boost/proto/args.hpp"
#include "third_party/boost/boost/proto/transform/impl.hpp"
#include "third_party/boost/boost/proto/detail/as_lvalue.hpp"
#include "third_party/boost/boost/proto/detail/ignore_unused.hpp"

namespace boost { namespace proto
{
    namespace detail
    {
        template<typename T>
        struct is_applyable
          : mpl::and_<is_callable<T>, is_transform<T> >
        {};

        template<typename T, bool HasType = mpl::aux::has_type<T>::value>
        struct nested_type
        {
            typedef typename T::type type;
        };

        template<typename T>
        struct nested_type<T, false>
        {
            typedef T type;
        };

        template<typename T, bool Applied>
        struct nested_type_if
        {
            typedef T type;
            static bool const applied = false;
        };

        template<typename T>
        struct nested_type_if<T, true>
          : nested_type<T>
        {
            static bool const applied = true;
        };

        template<
            typename R
          , typename Expr, typename State, typename Data
            BOOST_PROTO_TEMPLATE_ARITY_PARAM(long Arity = detail::template_arity<R>::value)
        >
        struct make_
        {
            typedef R type;
            static bool const applied = false;
        };

        template<
            typename R
          , typename Expr, typename State, typename Data
          , bool IsApplyable = is_applyable<R>::value
        >
        struct make_if_
          : make_<R, Expr, State, Data>
        {};

        template<typename R, typename Expr, typename State, typename Data>
        struct make_if_<R, Expr, State, Data, true>
          : uncvref<typename when<_, R>::template impl<Expr, State, Data>::result_type>
        {
            static bool const applied = true;
        };

        #if BOOST_WORKAROUND(__GNUC__, == 3) || (__GNUC__ == 4 && __GNUC_MINOR__ == 0)
        // work around GCC bug
        template<typename Tag, typename Args, long N, typename Expr, typename State, typename Data>
        struct make_if_<proto::expr<Tag, Args, N>, Expr, State, Data, false>
        {
            typedef proto::expr<Tag, Args, N> type;
            static bool const applied = false;
        };

        // work around GCC bug
        template<typename Tag, typename Args, long N, typename Expr, typename State, typename Data>
        struct make_if_<proto::basic_expr<Tag, Args, N>, Expr, State, Data, false>
        {
            typedef proto::basic_expr<Tag, Args, N> type;
            static bool const applied = false;
        };
        #endif

        template<typename Type, bool IsAggregate = detail::is_aggregate_<Type>::value>
        struct construct_
        {
            typedef Type result_type;

            BOOST_FORCEINLINE
            Type operator ()() const
            {
                return Type();
            }

            // Other overloads generated by the preprocessor
            #include "third_party/boost/boost/proto/transform/detail/construct_funop.hpp"
        };

        template<typename Type>
        struct construct_<Type, true>
        {
            typedef Type result_type;

            BOOST_FORCEINLINE
            Type operator ()() const
            {
                return Type();
            }

            // Other overloads generated by the preprocessor
            #include "third_party/boost/boost/proto/transform/detail/construct_pod_funop.hpp"
        };

    }

    /// \brief A PrimitiveTransform which prevents another PrimitiveTransform
    /// from being applied in an \c ObjectTransform.
    ///
    /// When building higher order transforms with <tt>make\<\></tt> or
    /// <tt>lazy\<\></tt>, you sometimes would like to build types that
    /// are parameterized with Proto transforms. In such lambda-style
    /// transforms, Proto will unhelpfully find all nested transforms
    /// and apply them, even if you don't want them to be applied. Consider
    /// the following transform, which will replace the \c _ in
    /// <tt>Bar<_>()</tt> with <tt>proto::terminal\<int\>::type</tt>:
    ///
    /// \code
    /// template<typename T>
    /// struct Bar
    /// {};
    /// 
    /// struct Foo
    ///   : proto::when<_, Bar<_>() >
    /// {};
    /// 
    /// proto::terminal<int>::type i = {0};
    /// 
    /// int main()
    /// {
    ///     Foo()(i);
    ///     std::cout << typeid(Foo()(i)).name() << std::endl;
    /// }
    /// \endcode
    ///
    /// If you actually wanted to default-construct an object of type
    /// <tt>Bar\<_\></tt>, you would have to protect the \c _ to prevent
    /// it from being applied. You can use <tt>proto::protect\<\></tt>
    /// as follows:
    ///
    /// \code
    /// // OK: replace anything with Bar<_>()
    /// struct Foo
    ///   : proto::when<_, Bar<protect<_> >() >
    /// {};
    /// \endcode
    template<typename PrimitiveTransform>
    struct protect : transform<protect<PrimitiveTransform> >
    {
        template<typename, typename, typename>
        struct impl
        {
            typedef PrimitiveTransform result_type;
        };
    };

    /// \brief A PrimitiveTransform which computes a type by evaluating any
    /// nested transforms and then constructs an object of that type.
    ///
    /// The <tt>make\<\></tt> transform checks to see if \c Object is a template.
    /// If it is, the template type is disassembled to find nested transforms.
    /// Proto considers the following types to represent transforms:
    ///
    /// \li Function types
    /// \li Function pointer types
    /// \li Types for which <tt>proto::is_callable\< type \>::value</tt> is \c true
    ///
    /// <tt>boost::result_of\<make\<T\<X0,X1,...\> \>(Expr, State, Data)\>::type</tt>
    /// is evaluated as follows. For each \c X in <tt>X0,X1,...</tt>, do:
    ///
    /// \li If \c X is a template like <tt>U\<Y0,Y1,...\></tt>, then let <tt>X'</tt>
    ///     be <tt>boost::result_of\<make\<U\<Y0,Y1,...\> \>(Expr, State, Data)\>::type</tt>
    ///     (which evaluates this procedure recursively). Note whether any
    ///     substitutions took place during this operation.
    /// \li Otherwise, if \c X is a transform, then let <tt>X'</tt> be
    ///     <tt>boost::result_of\<when\<_, X\>(Expr, State, Data)\>::type</tt>.
    ///     Note that a substitution took place.
    /// \li Otherwise, let <tt>X'</tt> be \c X, and note that no substitution
    ///     took place.
    /// \li If any substitutions took place in any of the above steps and
    ///     <tt>T\<X0',X1',...\></tt> has a nested <tt>::type</tt> typedef,
    ///     the result type is <tt>T\<X0',X1',...\>::type</tt>.
    /// \li Otherwise, the result type is <tt>T\<X0',X1',...\></tt>.
    ///
    /// Note that <tt>when\<\></tt> is implemented in terms of <tt>call\<\></tt>
    /// and <tt>make\<\></tt>, so the above procedure is evaluated recursively.
    template<typename Object>
    struct make : transform<make<Object> >
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef typename detail::make_if_<Object, Expr, State, Data>::type result_type;

            /// \return <tt>result_type()</tt>
            BOOST_FORCEINLINE
            result_type operator ()(
                typename impl::expr_param
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return result_type();
            }
        };
    };

    // Other specializations generated by the preprocessor.
    #include "third_party/boost/boost/proto/transform/detail/make.hpp"
    #include "third_party/boost/boost/proto/transform/detail/make_gcc_workaround.hpp"

    /// INTERNAL ONLY
    ///
    template<typename Object>
    struct is_callable<make<Object> >
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<typename PrimitiveTransform>
    struct is_callable<protect<PrimitiveTransform> >
      : mpl::true_
    {};

}}

#endif
