// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_GEOMETRY_POLICIES_RELATE_INTERSECTION_POINTS_HPP
#define BOOST_GEOMETRY_GEOMETRY_POLICIES_RELATE_INTERSECTION_POINTS_HPP


#include <algorithm>
#include <string>

#include "third_party/boost/boost/concept_check.hpp"
#include "third_party/boost/boost/numeric/conversion/cast.hpp"

#include "third_party/boost/boost/geometry/arithmetic/determinant.hpp"
#include "third_party/boost/boost/geometry/core/access.hpp"
#include "third_party/boost/boost/geometry/strategies/side_info.hpp"
#include "third_party/boost/boost/geometry/util/select_calculation_type.hpp"
#include "third_party/boost/boost/geometry/util/select_most_precise.hpp"


namespace boost { namespace geometry
{

namespace policies { namespace relate
{


template <typename S1, typename S2, typename ReturnType, typename CalculationType = void>
struct segments_intersection_points
{
    typedef ReturnType return_type;
    typedef S1 segment_type1;
    typedef S2 segment_type2;
    typedef typename select_calculation_type
        <
            S1, S2, CalculationType
        >::type coordinate_type;

    static inline return_type segments_intersect(side_info const&,
                    coordinate_type const& dx1, coordinate_type const& dy1,
                    coordinate_type const& dx2, coordinate_type const& dy2,
                    S1 const& s1, S2 const& s2)
    {
        return_type result;
        typedef typename geometry::coordinate_type
            <
                typename return_type::point_type
            >::type coordinate_type;

        // Get the same type, but at least a double (also used for divisions)
        typedef typename select_most_precise
            <
                coordinate_type, double
            >::type promoted_type;

        promoted_type const s1x = get<0, 0>(s1);
        promoted_type const s1y = get<0, 1>(s1);

        // Calculate other determinants - Cramers rule
        promoted_type const wx = get<0, 0>(s1) - get<0, 0>(s2);
        promoted_type const wy = get<0, 1>(s1) - get<0, 1>(s2);
        promoted_type const d = detail::determinant<promoted_type>(dx1, dy1, dx2, dy2);
        promoted_type const da = detail::determinant<promoted_type>(dx2, dy2, wx, wy);

        // r: ratio 0-1 where intersection divides A/B
        promoted_type r = da / d;
        promoted_type const zero = promoted_type();
        promoted_type const one = 1;
		// Handle robustness issues
		if (r < zero)
		{
			r = zero;
		}
		else if (r > one)
		{
			r = one;
		}

        result.count = 1;
        set<0>(result.intersections[0],
            boost::numeric_cast<coordinate_type>(s1x + r * promoted_type(dx1)));
        set<1>(result.intersections[0],
            boost::numeric_cast<coordinate_type>(s1y + r * promoted_type(dy1)));

        return result;
    }

    static inline return_type collinear_touch(coordinate_type const& x,
                coordinate_type const& y, int, int)
    {
        return_type result;
        result.count = 1;
        set<0>(result.intersections[0], x);
        set<1>(result.intersections[0], y);
        return result;
    }

    template <typename S>
    static inline return_type collinear_inside(S const& s, int index1 = 0, int index2 = 1)
    {
        return_type result;
        result.count = 2;
        set<0>(result.intersections[index1], get<0, 0>(s));
        set<1>(result.intersections[index1], get<0, 1>(s));
        set<0>(result.intersections[index2], get<1, 0>(s));
        set<1>(result.intersections[index2], get<1, 1>(s));
        return result;
    }

    template <typename S>
    static inline return_type collinear_interior_boundary_intersect(S const& s, bool a_in_b,
                int, int, bool opposite)
    {
        int index1 = opposite && ! a_in_b ? 1 : 0;
        return collinear_inside(s, index1, 1 - index1);
    }

    static inline return_type collinear_a_in_b(S1 const& s, bool)
    {
        return collinear_inside(s);
    }
    static inline return_type collinear_b_in_a(S2 const& s, bool opposite)
    {
        int index1 = opposite ? 1 : 0;
        return collinear_inside(s, index1, 1 - index1);
    }

    static inline return_type collinear_overlaps(
                coordinate_type const& x1, coordinate_type const& y1,
                coordinate_type const& x2, coordinate_type const& y2,
                int, int, bool)
    {
        return_type result;
        result.count = 2;
        set<0>(result.intersections[0], x1);
        set<1>(result.intersections[0], y1);
        set<0>(result.intersections[1], x2);
        set<1>(result.intersections[1], y2);
        return result;
    }

    static inline return_type segment_equal(S1 const& s, bool)
    {
        return_type result;
        result.count = 2;
        // TODO: order of IP's
        set<0>(result.intersections[0], get<0, 0>(s));
        set<1>(result.intersections[0], get<0, 1>(s));
        set<0>(result.intersections[1], get<1, 0>(s));
        set<1>(result.intersections[1], get<1, 1>(s));
        return result;
    }

    static inline return_type disjoint()
    {
        return return_type();
    }
    static inline return_type error(std::string const&)
    {
        return return_type();
    }

    static inline return_type collinear_disjoint()
    {
        return return_type();
    }

    static inline return_type degenerate(S1 const& s, bool)
    {
        return_type result;
        result.count = 1;
        set<0>(result.intersections[0], get<0, 0>(s));
        set<1>(result.intersections[0], get<0, 1>(s));
        return result;
    }
};


}} // namespace policies::relate

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_GEOMETRY_POLICIES_RELATE_INTERSECTION_POINTS_HPP
