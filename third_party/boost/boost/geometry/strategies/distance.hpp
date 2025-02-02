// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP
#define BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP


#include "third_party/boost/boost/mpl/assert.hpp"

#include "third_party/boost/boost/geometry/core/cs.hpp"
#include "third_party/boost/boost/geometry/strategies/tags.hpp"


namespace boost { namespace geometry
{


namespace strategy { namespace distance { namespace services
{


template <typename Strategy> struct tag {};
template <typename Strategy> struct return_type
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_STRATEGY, (types<Strategy>)
        );
};


/*!
    \brief Metafunction delivering a similar strategy with other input point types
*/
template
<
    typename Strategy,
    typename Point1,
    typename Point2
>
struct similar_type
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_STRATEGY
            , (types<Strategy, Point1, Point2>)
        );
};

template
<
    typename Strategy,
    typename Point1,
    typename Point2
>
struct get_similar
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_STRATEGY
            , (types<Strategy, Point1, Point2>)
        );
};

template <typename Strategy> struct comparable_type
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_STRATEGY, (types<Strategy>)
        );
};

template <typename Strategy> struct get_comparable
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_STRATEGY, (types<Strategy>)
        );
};

template <typename Strategy> struct result_from_distance {};


// For point-segment only:
template <typename Strategy> struct strategy_point_point {};


// Default strategy


/*!
    \brief Traits class binding a default strategy for distance
        to one (or possibly two) coordinate system(s)
    \ingroup distance
    \tparam GeometryTag tag (point/segment) for which this strategy is the default
    \tparam Point1 first point-type
    \tparam Point2 second point-type
    \tparam CsTag1 tag of coordinate system of first point type
    \tparam CsTag2 tag of coordinate system of second point type
*/
template
<
    typename GeometryTag,
    typename Point1,
    typename Point2 = Point1,
    typename CsTag1 = typename cs_tag<Point1>::type,
    typename CsTag2 = typename cs_tag<Point2>::type,
    typename UnderlyingStrategy = void
>
struct default_strategy
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_POINT_TYPE_COMBINATION
            , (types<Point1, Point2, CsTag1, CsTag2>)
        );
};


}}} // namespace strategy::distance::services


}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP
