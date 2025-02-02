// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef BOOST_RANGE_ALGORITHM_EXT_INSERT_HPP_INCLUDED
#define BOOST_RANGE_ALGORITHM_EXT_INSERT_HPP_INCLUDED

#include "third_party/boost/boost/range/config.hpp"
#include "third_party/boost/boost/range/concepts.hpp"
#include "third_party/boost/boost/range/difference_type.hpp"
#include "third_party/boost/boost/range/begin.hpp"
#include "third_party/boost/boost/range/end.hpp"
#include "third_party/boost/boost/assert.hpp"

namespace boost
{
    namespace range
    {

template< class Container, class Range >
inline Container& insert( Container& on,
                          BOOST_DEDUCED_TYPENAME Container::iterator before,
                          const Range& from )
{
    BOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    BOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<Range> ));
    BOOST_ASSERT( (void*)&on != (void*)&from &&
                  "cannot copy from a container to itself" );
    on.insert( before, boost::begin(from), boost::end(from) );
    return on;
}

    } // namespace range
    using range::insert;
} // namespace boost

#endif // include guard
