// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_POINTER_TYPE_HPP
#define BOOST_RANGE_POINTER_TYPE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "third_party/boost/boost/range/config.hpp"
#include "third_party/boost/boost/range/iterator.hpp"
#include "third_party/boost/boost/iterator/iterator_traits.hpp"

namespace boost
{
    template< class T >
    struct range_pointer : iterator_pointer< typename range_iterator<T>::type >
    { };
}

#endif
