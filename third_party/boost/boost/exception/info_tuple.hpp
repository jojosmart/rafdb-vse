//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_63EE924290FB11DC87BB856555D89593
#define UUID_63EE924290FB11DC87BB856555D89593
#if defined(__GNUC__) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif
#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(push,1)
#endif

#include "third_party/boost/boost/exception/info.hpp"
#include "third_party/boost/boost/tuple/tuple.hpp"

namespace
boost
    {
    template <
        class E,
        class Tag1,class T1,
        class Tag2,class T2 >
    inline
    E const &
    operator<<(
        E const & x,
        tuple<
            error_info<Tag1,T1>,
            error_info<Tag2,T2> > const & v )
        {
        return x << v.template get<0>() << v.template get<1>();
        }

    template <
        class E,
        class Tag1,class T1,
        class Tag2,class T2,
        class Tag3,class T3 >
    inline
    E const &
    operator<<(
        E const & x,
        tuple<
            error_info<Tag1,T1>,
            error_info<Tag2,T2>,
            error_info<Tag3,T3> > const & v )
        {
        return x << v.template get<0>() << v.template get<1>() << v.template get<2>();
        }

    template <
        class E,
        class Tag1,class T1,
        class Tag2,class T2,
        class Tag3,class T3,
        class Tag4,class T4 >
    inline
    E const &
    operator<<(
        E const & x,
        tuple<
            error_info<Tag1,T1>,
            error_info<Tag2,T2>,
            error_info<Tag3,T3>,
            error_info<Tag4,T4> > const & v )
        {
        return x << v.template get<0>() << v.template get<1>() << v.template get<2>() << v.template get<3>();
        }
    }

#if defined(_MSC_VER) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma warning(pop)
#endif
#endif
