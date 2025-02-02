// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_ALGORITHMS_DETAIL_OVERLAY_SELF_TURN_POINTS_HPP
#define BOOST_GEOMETRY_MULTI_ALGORITHMS_DETAIL_OVERLAY_SELF_TURN_POINTS_HPP


#include "third_party/boost/boost/geometry/multi/core/tags.hpp"
#include "third_party/boost/boost/geometry/algorithms/detail/overlay/self_turn_points.hpp"


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template
<
    typename MultiPolygon,
    typename Turns,
    typename TurnPolicy,
    typename InterruptPolicy
>
struct self_get_turn_points
    <
        multi_polygon_tag, MultiPolygon,
        Turns,
        TurnPolicy, InterruptPolicy
    >
    : detail::self_get_turn_points::get_turns
        <
            MultiPolygon,
            Turns,
            TurnPolicy,
            InterruptPolicy
        >
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_ALGORITHMS_DETAIL_OVERLAY_SELF_TURN_POINTS_HPP
