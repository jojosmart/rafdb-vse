// Copyright 2005 Daniel Wallin.
// Copyright 2005 Joel de Guzman.
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Modeled after range_ex, Copyright 2004 Eric Niebler
///////////////////////////////////////////////////////////////////////////////
//
// std_hash_map_fwd.hpp
//
/////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && _MSC_VER >= 1000
# pragma once
#endif

#ifndef STD_HASH_MAP_FWD_EN_16_12_2004
#define STD_HASH_MAP_FWD_EN_16_12_2004

#include "third_party/boost/boost/config.hpp"

#if defined(BOOST_HAS_HASH)

namespace BOOST_STD_EXTENSION_NAMESPACE
{
    template<
        class Kty
      , class Ty
      , class Hash
      , class Cmp
      , class Alloc
    >
    class hash_map;

    template<
        class Kty
      , class Ty
      , class Hash
      , class Cmp
      , class Alloc
    >
    class hash_multimap;
}

#elif defined(BOOST_DINKUMWARE_STDLIB)

namespace BOOST_STD_EXTENSION_NAMESPACE
{
    template<
        class Kty
      , class Ty
      , class Tr
      , class Alloc
    >
    class hash_map;

    template<
        class Kty
      , class Ty
      , class Tr
      , class Alloc
    >
    class hash_multimap;
}

#endif

#endif
