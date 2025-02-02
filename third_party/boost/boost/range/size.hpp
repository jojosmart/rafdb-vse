// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_SIZE_HPP
#define BOOST_RANGE_SIZE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "third_party/boost/boost/range/config.hpp"
#include "third_party/boost/boost/range/begin.hpp"
#include "third_party/boost/boost/range/end.hpp"
#include "third_party/boost/boost/range/difference_type.hpp"
#include "third_party/boost/boost/assert.hpp"

namespace boost
{
    namespace range_detail
    {
        template<class SinglePassRange>
        inline BOOST_DEDUCED_TYPENAME range_difference<SinglePassRange>::type
        range_calculate_size(const SinglePassRange& rng)
        {
            BOOST_ASSERT( (boost::end(rng) - boost::begin(rng)) >= 0 &&
                          "reachability invariant broken!" );
            return boost::end(rng) - boost::begin(rng);
        }
    }

    template<class SinglePassRange>
    inline BOOST_DEDUCED_TYPENAME range_difference<SinglePassRange>::type
    size(const SinglePassRange& rng)
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564)) && \
    !BOOST_WORKAROUND(__GNUC__, < 3) \
    /**/
        using namespace range_detail;
#endif
        return range_calculate_size(rng);
    }

} // namespace 'boost'

#endif
