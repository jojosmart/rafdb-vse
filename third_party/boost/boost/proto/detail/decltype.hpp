///////////////////////////////////////////////////////////////////////////////
/// \file decltype.hpp
/// Contains definition the BOOST_PROTO_DECLTYPE_() macro and assorted helpers
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DETAIL_DECLTYPE_HPP_EAN_04_04_2008
#define BOOST_PROTO_DETAIL_DECLTYPE_HPP_EAN_04_04_2008

#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/get_pointer.hpp"
#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"
#include "third_party/boost/boost/preprocessor/iteration/local.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"
#include "third_party/boost/boost/type_traits/is_class.hpp"
#include "third_party/boost/boost/type_traits/remove_reference.hpp"
#include "third_party/boost/boost/type_traits/is_pointer.hpp"
#include "third_party/boost/boost/type_traits/is_function.hpp"
#include "third_party/boost/boost/type_traits/is_member_object_pointer.hpp"
#include "third_party/boost/boost/type_traits/add_const.hpp"
#include "third_party/boost/boost/type_traits/add_reference.hpp"
#include "third_party/boost/boost/typeof/typeof.hpp"
#include "third_party/boost/boost/utility/addressof.hpp"
#include "third_party/boost/boost/utility/result_of.hpp"
#include "third_party/boost/boost/utility/enable_if.hpp"

#ifndef BOOST_NO_DECLTYPE
# define BOOST_PROTO_DECLTYPE_(EXPR, TYPE) typedef decltype(EXPR) TYPE;
#else
# define BOOST_PROTO_DECLTYPE_NESTED_TYPEDEF_TPL_(NESTED, EXPR)                                     \
    BOOST_TYPEOF_NESTED_TYPEDEF_TPL(BOOST_PP_CAT(nested_and_hidden_, NESTED), EXPR)                 \
    static int const BOOST_PP_CAT(sz, NESTED) = sizeof(boost::proto::detail::check_reference(EXPR));\
    struct NESTED                                                                                   \
      : boost::mpl::if_c<                                                                           \
            1 == BOOST_PP_CAT(sz, NESTED)                                                           \
          , typename BOOST_PP_CAT(nested_and_hidden_, NESTED)::type &                               \
          , typename BOOST_PP_CAT(nested_and_hidden_, NESTED)::type                                 \
        >                                                                                           \
    {};
# define BOOST_PROTO_DECLTYPE_(EXPR, TYPE)                                                          \
    BOOST_PROTO_DECLTYPE_NESTED_TYPEDEF_TPL_(BOOST_PP_CAT(nested_, TYPE), (EXPR))                   \
    typedef typename BOOST_PP_CAT(nested_, TYPE)::type TYPE;
#endif

namespace boost { namespace proto
{
    namespace detail
    {
        namespace anyns
        {
            ////////////////////////////////////////////////////////////////////////////////////////////
            struct any
            {
                any(...);
                any operator=(any);
                any operator[](any);
                #define M0(Z, N, DATA) any operator()(BOOST_PP_ENUM_PARAMS_Z(Z, N, any BOOST_PP_INTERCEPT));
                BOOST_PP_REPEAT(BOOST_PROTO_MAX_ARITY, M0, ~)
                #undef M0

                template<typename T>
                operator T &() const volatile;

                any operator+();
                any operator-();
                any operator*();
                any operator&();
                any operator~();
                any operator!();
                any operator++();
                any operator--();
                any operator++(int);
                any operator--(int);

                friend any operator<<(any, any);
                friend any operator>>(any, any);
                friend any operator*(any, any);
                friend any operator/(any, any);
                friend any operator%(any, any);
                friend any operator+(any, any);
                friend any operator-(any, any);
                friend any operator<(any, any);
                friend any operator>(any, any);
                friend any operator<=(any, any);
                friend any operator>=(any, any);
                friend any operator==(any, any);
                friend any operator!=(any, any);
                friend any operator||(any, any);
                friend any operator&&(any, any);
                friend any operator&(any, any);
                friend any operator|(any, any);
                friend any operator^(any, any);
                friend any operator,(any, any);
                friend any operator->*(any, any);

                friend any operator<<=(any, any);
                friend any operator>>=(any, any);
                friend any operator*=(any, any);
                friend any operator/=(any, any);
                friend any operator%=(any, any);
                friend any operator+=(any, any);
                friend any operator-=(any, any);
                friend any operator&=(any, any);
                friend any operator|=(any, any);
                friend any operator^=(any, any);
            };
        }

        using anyns::any;

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        struct as_mutable
        {
            typedef T &type;
        };

        template<typename T>
        struct as_mutable<T &>
        {
            typedef T &type;
        };

        template<typename T>
        struct as_mutable<T const &>
        {
            typedef T &type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        T make();

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        typename as_mutable<T>::type make_mutable();

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        struct subscript_wrapper
          : T
        {
            using T::operator[];

            #if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1500))
            any operator[](any const volatile &) const volatile;
            #else
            any operator[](any const &) const volatile;
            #endif
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        struct as_subscriptable
        {
            typedef
                typename mpl::if_c<
                    is_class<T>::value
                  , subscript_wrapper<T>
                  , T
                >::type
            type;
        };

        template<typename T>
        struct as_subscriptable<T const>
        {
            typedef
                typename mpl::if_c<
                    is_class<T>::value
                  , subscript_wrapper<T> const
                  , T const
                >::type
            type;
        };

        template<typename T>
        struct as_subscriptable<T &>
        {
            typedef
                typename mpl::if_c<
                    is_class<T>::value
                  , subscript_wrapper<T> &
                  , T &
                >::type
            type;
        };

        template<typename T>
        struct as_subscriptable<T const &>
        {
            typedef
                typename mpl::if_c<
                    is_class<T>::value
                  , subscript_wrapper<T> const &
                  , T const &
                >::type
            type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        typename as_subscriptable<T>::type make_subscriptable();

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        char check_reference(T &);

        template<typename T>
        char (&check_reference(T const &))[2];

        namespace has_get_pointerns
        {
            using boost::get_pointer;
            void *(&get_pointer(...))[2];

            ////////////////////////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct has_get_pointer
            {
                static const bool value = sizeof(void *) == sizeof(get_pointer(make<T &>()));
                typedef mpl::bool_<value> type;
            };
        }

        using has_get_pointerns::has_get_pointer;

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        struct classtypeof;

        template<typename T, typename U>
        struct classtypeof<T U::*>
        {
            typedef U type;
        };

        // Other specializations are generated by the preprocessor
        #include "third_party/boost/boost/proto/detail/classtypeof.hpp"

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        T &lvalue(T &t)
        {
            return t;
        }

        template<typename T>
        T const &lvalue(T const &t)
        {
            return t;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename U, typename V, typename T>
        U *proto_get_pointer(T &t, V *, U *)
        {
            return boost::addressof(t);
        }

        template<typename U, typename V, typename T>
        U const *proto_get_pointer(T &t, V *, U const *)
        {
            return boost::addressof(t);
        }

        template<typename U, typename V, typename T>
        V *proto_get_pointer(T &t, V *, ...)
        {
            return get_pointer(t);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        #define BOOST_PROTO_USE_GET_POINTER()                                                       \
        using namespace boost::proto::detail::get_pointerns                                         \
        /**/

        #define BOOST_PROTO_GET_POINTER(Type, Obj)                                                  \
        boost::proto::detail::proto_get_pointer<Type>(                                              \
            boost::proto::detail::lvalue(Obj)                                                       \
          , (true ? 0 : get_pointer(Obj))                                                           \
          , (true ? 0 : boost::addressof(boost::proto::detail::lvalue(Obj)))                        \
        )                                                                                           \
        /**/

        ////////////////////////////////////////////////////////////////////////////////////////////
        namespace get_pointerns
        {
            using boost::get_pointer;

            template<typename T>
            typename disable_if_c<has_get_pointer<T>::value, T *>::type
            get_pointer(T &t)
            {
                return boost::addressof(t);
            }

            template<typename T>
            typename disable_if_c<has_get_pointer<T>::value, T const *>::type
            get_pointer(T const &t)
            {
                return boost::addressof(t);
            }

            char test_ptr_to_const(void *);
            char (&test_ptr_to_const(void const *))[2];

            template<typename U> char test_V_is_a_U(U *);
            template<typename U> char test_V_is_a_U(U const *);
            template<typename U> char (&test_V_is_a_U(...))[2];

            ////////////////////////////////////////////////////////////////////////////////////////////
            // result_of_ is a wrapper around boost::result_of that also handles "invocations" of
            // member object pointers.
            template<typename T, typename Void = void>
            struct result_of_
              : BOOST_PROTO_RESULT_OF<T>
            {};

            template<typename T, typename U, typename V>
            struct result_of_<T U::*(V), typename enable_if_c<is_member_object_pointer<T U::*>::value>::type>
            {
                static const bool is_V_a_smart_ptr = 2 == sizeof(test_V_is_a_U<U>(&lvalue(make<V>())));
                static const bool is_ptr_to_const = 2 == sizeof(test_ptr_to_const(BOOST_PROTO_GET_POINTER(U, make<V>())));

                // If V is not a U, then it is a (smart) pointer and we can always return an lvalue.
                // Otherwise, we can only return an lvalue if we are given one.
                typedef
                    typename mpl::eval_if_c<
                        (is_V_a_smart_ptr || is_reference<V>::value)
                      , mpl::eval_if_c<
                            is_ptr_to_const
                          , add_reference<typename add_const<T>::type>
                          , add_reference<T>
                        >
                      , mpl::identity<T>
                    >::type
                type;
            };

            ////////////////////////////////////////////////////////////////////////////////////////////
            template<
                typename T
              , typename U
              , bool IsMemPtr = is_member_object_pointer<
                    typename remove_reference<U>::type
                >::value
            >
            struct mem_ptr_fun
            {
                BOOST_PROTO_DECLTYPE_(
                    proto::detail::make_mutable<T>() ->* proto::detail::make<U>()
                  , result_type
                )

                result_type operator()(
                    typename add_reference<typename add_const<T>::type>::type t
                  , typename add_reference<typename add_const<U>::type>::type u
                ) const
                {
                    return t ->* u;
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////////
            template<typename T, typename U>
            struct mem_ptr_fun<T, U, true>
            {
                typedef
                    typename classtypeof<
                        typename uncvref<U>::type
                    >::type
                V;

                BOOST_PROTO_DECLTYPE_(
                    BOOST_PROTO_GET_POINTER(V, proto::detail::make_mutable<T>()) ->* proto::detail::make<U>()
                  , result_type
                )

                result_type operator()(
                    typename add_reference<typename add_const<T>::type>::type t
                  , U u
                ) const
                {
                    return BOOST_PROTO_GET_POINTER(V, t) ->* u;
                }
            };
        }

        using get_pointerns::result_of_;
        using get_pointerns::mem_ptr_fun;

        ////////////////////////////////////////////////////////////////////////////////////////////
        template<typename A0, typename A1>
        struct comma_result
        {
            BOOST_PROTO_DECLTYPE_((proto::detail::make<A0>(), proto::detail::make<A1>()), type)
        };

        template<typename A0>
        struct comma_result<A0, void>
        {
            typedef void type;
        };

        template<typename A1>
        struct comma_result<void, A1>
        {
            typedef A1 type;
        };

        template<>
        struct comma_result<void, void>
        {
            typedef void type;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////
        // normalize a function type for use with boost::result_of
        template<typename T, typename U = T>
        struct result_of_fixup
          : mpl::if_c<is_function<T>::value, T *, U>
        {};

        template<typename T, typename U>
        struct result_of_fixup<T &, U>
          : result_of_fixup<T, T>
        {};

        template<typename T, typename U>
        struct result_of_fixup<T const &, U>
          : result_of_fixup<T, T>
        {};

        template<typename T, typename U>
        struct result_of_fixup<T *, U>
          : result_of_fixup<T, U>
        {};

        template<typename R, typename T, typename U>
        struct result_of_fixup<R T::*, U>
        {
            typedef R T::*type;
        };

        template<typename T, typename U>
        struct result_of_fixup<T const, U>
          : result_of_fixup<T, U>
        {};

        //// Tests for result_of_fixup
        //struct bar {};
        //BOOST_MPL_ASSERT((is_same<bar,        result_of_fixup<bar>::type>));
        //BOOST_MPL_ASSERT((is_same<bar const,  result_of_fixup<bar const>::type>));
        //BOOST_MPL_ASSERT((is_same<bar,        result_of_fixup<bar &>::type>));
        //BOOST_MPL_ASSERT((is_same<bar const,  result_of_fixup<bar const &>::type>));
        //BOOST_MPL_ASSERT((is_same<void(*)(),  result_of_fixup<void(*)()>::type>));
        //BOOST_MPL_ASSERT((is_same<void(*)(),  result_of_fixup<void(* const)()>::type>));
        //BOOST_MPL_ASSERT((is_same<void(*)(),  result_of_fixup<void(* const &)()>::type>));
        //BOOST_MPL_ASSERT((is_same<void(*)(),  result_of_fixup<void(&)()>::type>));

        template<typename T, typename PMF>
        struct memfun
        {
            typedef typename uncvref<PMF>::type pmf_type;
            typedef typename classtypeof<pmf_type>::type V;
            typedef typename BOOST_PROTO_RESULT_OF<pmf_type(T)>::type result_type;

            memfun(T t, PMF p)
              : obj(t)
              , pmf(p)
            {}

            result_type operator()() const
            {
                BOOST_PROTO_USE_GET_POINTER();
                return (BOOST_PROTO_GET_POINTER(V, obj) ->* pmf)();
            }

            // Other overloads generated by the preprocessor
            #include "third_party/boost/boost/proto/detail/memfun_funop.hpp"

        private:
            T obj;
            PMF pmf;
        };

    } // namespace detail
}}

#endif
