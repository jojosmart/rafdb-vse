// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_EMPTY_HPP
#define BOOST_RANGE_EMPTY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "third_party/boost/boost/range/config.hpp"
#include "third_party/boost/boost/range/begin.hpp"
#include "third_party/boost/boost/range/end.hpp"

namespace boost 
{ 

    template< class T >
    inline bool empty( const T& r )
    {
        return boost::begin( r ) == boost::end( r );
    }

} // namepace 'boost'


#endif
