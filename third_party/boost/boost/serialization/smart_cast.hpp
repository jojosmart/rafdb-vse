#ifndef BOOST_SERIALIZATION_SMART_CAST_HPP
#define BOOST_SERIALIZATION_SMART_CAST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// smart_cast.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/serialization for updates, documentation, and revision history.

// casting of pointers and references.  

// In casting between different C++ classes, there are a number of
// rules that have to be kept in mind in deciding whether to use
// static_cast or dynamic_cast.  

// a) dynamic casting can only be applied when one of the types is polymorphic
// Otherwise static_cast must be used.
// b) only dynamic casting can do runtime error checking
// use of static_cast is generally un checked even when compiled for debug
// c) static_cast would be considered faster than dynamic_cast.

// If casting is applied to a template parameter, there is no apriori way
// to know which of the two casting methods will be permitted or convenient.

// smart_cast uses C++ type_traits, and program debug mode to select the
// most convenient cast to use.

#include <exception>
#include <typeinfo>
#include <cstddef> // NULL

#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/static_assert.hpp"

#include "third_party/boost/boost/type_traits/is_base_and_derived.hpp"
#include "third_party/boost/boost/type_traits/is_polymorphic.hpp"
#include "third_party/boost/boost/type_traits/is_pointer.hpp"
#include "third_party/boost/boost/type_traits/is_reference.hpp"
#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/type_traits/remove_pointer.hpp"
#include "third_party/boost/boost/type_traits/remove_reference.hpp"

#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/or.hpp"
#include "third_party/boost/boost/mpl/and.hpp"
#include "third_party/boost/boost/mpl/not.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"

namespace boost {
namespace serialization {
namespace smart_cast_impl {

    template<class T>
    struct reference {

        struct polymorphic {

            struct linear {
                template<class U>
                 static T cast(U & u){
                    return static_cast< T >(u);
                }
            };

            struct cross {
                 template<class U>
                static T cast(U & u){
                    return dynamic_cast< T >(u);
                }
            };

            template<class U>
            static T cast(U & u){
                // if we're in debug mode
                #if ! defined(NDEBUG)                               \
                || defined(__BORLANDC__) && (__BORLANDC__ <= 0x560) \
                || defined(__MWERKS__)
                    // do a checked dynamic cast
                    return cross::cast(u);
                #else
                    // borland 5.51 chokes here so we can't use it
                    // note: if remove_reference isn't function for these types
                    // cross casting will be selected this will work but will
                    // not be the most efficient method. This will conflict with
                    // the original smart_cast motivation.
                    typedef BOOST_DEDUCED_TYPENAME mpl::eval_if<
                            BOOST_DEDUCED_TYPENAME mpl::and_<
                                mpl::not_<is_base_and_derived<
                                    BOOST_DEDUCED_TYPENAME remove_reference< T >::type,
                                    U
                                > >,
                                mpl::not_<is_base_and_derived<
                                    U,
                                    BOOST_DEDUCED_TYPENAME remove_reference< T >::type
                                > >
                            >,
                            // borland chokes w/o full qualification here
                            mpl::identity<cross>,
                            mpl::identity<linear>
                    >::type typex;
                    // typex works around gcc 2.95 issue
                    return typex::cast(u);
                #endif
            }
        };

        struct non_polymorphic {
            template<class U>
             static T cast(U & u){
                return static_cast< T >(u);
            }
        };
        template<class U>
        static T cast(U & u){
            #if defined(__BORLANDC__)
                return mpl::eval_if<
                    boost::is_polymorphic<U>,
                    mpl::identity<polymorphic>,
                    mpl::identity<non_polymorphic>
                >::type::cast(u);
            #else
                typedef BOOST_DEDUCED_TYPENAME mpl::eval_if<
                    boost::is_polymorphic<U>,
                    mpl::identity<polymorphic>,
                    mpl::identity<non_polymorphic>
                >::type typex;
                return typex::cast(u);
            #endif
        }
    };

    template<class T>
    struct pointer {

        struct polymorphic {
            // unfortunately, this below fails to work for virtual base 
            // classes.  need has_virtual_base to do this.
            // Subject for further study
            #if 0
            struct linear {
                template<class U>
                 static T cast(U * u){
                    return static_cast< T >(u);
                }
            };

            struct cross {
                template<class U>
                static T cast(U * u){
                    T tmp = dynamic_cast< T >(u);
                    #ifndef NDEBUG
                        if ( tmp == 0 ) throw std::bad_cast();
                    #endif
                    return tmp;
                }
            };

            template<class U>
            static T cast(U * u){
                // if we're in debug mode
                #if ! defined(NDEBUG) || defined(__BORLANDC__) && (__BORLANDC__ <= 0x560)
                    // do a checked dynamic cast
                    return cross::cast(u);
                #else
                    // borland 5.51 chokes here so we can't use it
                    // note: if remove_pointer isn't function for these types
                    // cross casting will be selected this will work but will
                    // not be the most efficient method. This will conflict with
                    // the original smart_cast motivation.
                    typedef
                        BOOST_DEDUCED_TYPENAME mpl::eval_if<
                            BOOST_DEDUCED_TYPENAME mpl::and_<
                                mpl::not_<is_base_and_derived<
                                    BOOST_DEDUCED_TYPENAME remove_pointer< T >::type,
                                    U
                                > >,
                                mpl::not_<is_base_and_derived<
                                    U,
                                    BOOST_DEDUCED_TYPENAME remove_pointer< T >::type
                                > >
                            >,
                            // borland chokes w/o full qualification here
                            mpl::identity<cross>,
                            mpl::identity<linear>
                        >::type typex;
                    return typex::cast(u);
                #endif
            }
            #else
            template<class U>
            static T cast(U * u){
                T tmp = dynamic_cast< T >(u);
                #ifndef NDEBUG
                    if ( tmp == 0 ) throw std::bad_cast();
                #endif
                return tmp;
            }
            #endif
        };

        struct non_polymorphic {
            template<class U>
             static T cast(U * u){
                return static_cast< T >(u);
            }
        };

        template<class U>
        static T cast(U * u){
            #if defined(__BORLANDC__)
                return mpl::eval_if<
                    boost::is_polymorphic<U>,
                    mpl::identity<polymorphic>,
                    mpl::identity<non_polymorphic>
                >::type::cast(u);
            #else
                typedef BOOST_DEDUCED_TYPENAME mpl::eval_if<
                    boost::is_polymorphic<U>,
                    mpl::identity<polymorphic>,
                    mpl::identity<non_polymorphic>
                >::type typex;
                return typex::cast(u);
            #endif
        }

    };

    template<class TPtr>
    struct void_pointer {
        template<class UPtr>
        static TPtr cast(UPtr uptr){
            return static_cast<TPtr>(uptr);
        }
    };

    template<class T>
    struct error {
        // if we get here, its because we are using one argument in the
        // cast on a system which doesn't support partial template 
        // specialization
        template<class U>
        static T cast(U u){
            BOOST_STATIC_ASSERT(sizeof(T)==0);
            return * static_cast<T *>(NULL);
        }
    };

} // smart_cast_impl

// this implements:
// smart_cast<Target *, Source *>(Source * s)
// smart_cast<Target &, Source &>(s)
// note that it will fail with
// smart_cast<Target &>(s)
template<class T, class U>
T smart_cast(U u) {
    typedef
        BOOST_DEDUCED_TYPENAME mpl::eval_if<
            BOOST_DEDUCED_TYPENAME mpl::or_<
                boost::is_same<void *, U>,
                boost::is_same<void *, T>,
                boost::is_same<const void *, U>,
                boost::is_same<const void *, T>
            >,
            mpl::identity<smart_cast_impl::void_pointer< T > >,
        // else
        BOOST_DEDUCED_TYPENAME mpl::eval_if<boost::is_pointer<U>,
            mpl::identity<smart_cast_impl::pointer< T > >,
        // else
        BOOST_DEDUCED_TYPENAME mpl::eval_if<boost::is_reference<U>,
            mpl::identity<smart_cast_impl::reference< T > >,
        // else
            mpl::identity<smart_cast_impl::error< T >
        >
        >
        >
        >::type typex;
    return typex::cast(u);
}

// this implements:
// smart_cast_reference<Target &>(Source & s)
template<class T, class U>
T smart_cast_reference(U & u) {
    return smart_cast_impl::reference< T >::cast(u);
}

} // namespace serialization
} // namespace boost

#endif // BOOST_SERIALIZATION_SMART_CAST_HPP
