// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP
#define BOOST_GEOMETRY_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP


#include <vector>

#include "third_party/boost/boost/concept_check.hpp"


namespace boost { namespace geometry { namespace concept
{


/*!
    \brief Checks strategy for convex_hull
    \ingroup convex_hull
*/
template <typename Strategy>
class ConvexHullStrategy
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS

    // 1) must define state_type
    typedef typename Strategy::state_type state_type;

    // 2) must define point_type
    typedef typename Strategy::point_type point_type;

    // 3) must define geometry_type
    typedef typename Strategy::geometry_type geometry_type;

    struct check_methods
    {
        static void apply()
        {
            Strategy const* str;

            state_type* st;
            geometry_type* sp;
            std::vector<point_type> *v;

            // 4) must implement a method apply, iterating over a range
            str->apply(*sp, *st);

            // 5) must implement a method result, with an output iterator
            str->result(*st, std::back_inserter(*v), true);
        }
    };

public :
    BOOST_CONCEPT_USAGE(ConvexHullStrategy)
    {
        check_methods::apply();
    }
#endif
};


}}} // namespace boost::geometry::concept


#endif // BOOST_GEOMETRY_STRATEGIES_CONCEPTS_CONVEX_HULL_CONCEPT_HPP
