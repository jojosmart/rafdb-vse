#ifndef BOOST_SERIALIZATION_BASE_OBJECT_HPP
#define BOOST_SERIALIZATION_BASE_OBJECT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// base_object.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// if no archive headers have been included this is a no op
// this is to permit BOOST_EXPORT etc to be included in a 
// file declaration header

#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/detail/workaround.hpp"

#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/int.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"

#include "third_party/boost/boost/type_traits/is_base_and_derived.hpp"
#include "third_party/boost/boost/type_traits/is_pointer.hpp"
#include "third_party/boost/boost/type_traits/is_const.hpp"
#include "third_party/boost/boost/type_traits/is_polymorphic.hpp"

#include "third_party/boost/boost/static_assert.hpp"
#include "third_party/boost/boost/serialization/access.hpp"
#include "third_party/boost/boost/serialization/force_include.hpp"
#include "third_party/boost/boost/serialization/void_cast_fwd.hpp"

namespace boost {
namespace serialization {

namespace detail
{
    // get the base type for a given derived type
    // preserving the const-ness
    template<class B, class D>
    struct base_cast
    {
        typedef BOOST_DEDUCED_TYPENAME
        mpl::if_<
            is_const<D>,
            const B,
            B
        >::type type;
        BOOST_STATIC_ASSERT(is_const<type>::value == is_const<D>::value);
    };

    // only register void casts if the types are polymorphic
    template<class Base, class Derived>
    struct base_register
    {
        struct polymorphic {
            static void const * invoke(){
                Base const * const b = 0;
                Derived const * const d = 0;
                return & void_cast_register(d, b);
            }
        };
        struct non_polymorphic {
            static void const * invoke(){
                return 0;
            }
        };
        static void const * invoke(){
            typedef BOOST_DEDUCED_TYPENAME mpl::eval_if<
                is_polymorphic<Base>,
                mpl::identity<polymorphic>,
                mpl::identity<non_polymorphic>
            >::type type;
            return type::invoke();
        }
    };

} // namespace detail
#if defined(__BORLANDC__) && __BORLANDC__ < 0x610
template<class Base, class Derived>
const Base & 
base_object(const Derived & d)
{
    BOOST_STATIC_ASSERT(! is_pointer<Derived>::value);
    detail::base_register<Base, Derived>::invoke();
    return access::cast_reference<const Base, Derived>(d);
}
#else
template<class Base, class Derived>
BOOST_DEDUCED_TYPENAME detail::base_cast<Base, Derived>::type & 
base_object(Derived &d)
{
    BOOST_STATIC_ASSERT(( is_base_and_derived<Base,Derived>::value));
    BOOST_STATIC_ASSERT(! is_pointer<Derived>::value);
    typedef BOOST_DEDUCED_TYPENAME detail::base_cast<Base, Derived>::type type;
    detail::base_register<type, Derived>::invoke();
    return access::cast_reference<type, Derived>(d);
}
#endif

} // namespace serialization
} // namespace boost

#endif // BOOST_SERIALIZATION_BASE_OBJECT_HPP
