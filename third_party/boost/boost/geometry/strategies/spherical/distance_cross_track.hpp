// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_SPHERICAL_DISTANCE_CROSS_TRACK_HPP
#define BOOST_GEOMETRY_STRATEGIES_SPHERICAL_DISTANCE_CROSS_TRACK_HPP


#include "third_party/boost/boost/concept_check.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/type_traits.hpp"

#include "third_party/boost/boost/geometry/core/cs.hpp"
#include "third_party/boost/boost/geometry/core/access.hpp"
#include "third_party/boost/boost/geometry/core/radian_access.hpp"


#include "third_party/boost/boost/geometry/strategies/distance.hpp"
#include "third_party/boost/boost/geometry/strategies/concepts/distance_concept.hpp"

#include "third_party/boost/boost/geometry/util/promote_floating_point.hpp"
#include "third_party/boost/boost/geometry/util/math.hpp"

#ifdef BOOST_GEOMETRY_DEBUG_CROSS_TRACK
#  include "third_party/boost/boost/geometry/io/dsv/write.hpp"
#endif



namespace boost { namespace geometry
{

namespace strategy { namespace distance
{

/*!
\brief Strategy functor for distance point to segment calculation
\ingroup strategies
\details Class which calculates the distance of a point to a segment, using latlong points
\see http://williams.best.vwh.net/avform.htm
\tparam Point point type
\tparam PointOfSegment \tparam_segment_point
\tparam CalculationType \tparam_calculation
\tparam Strategy underlying point-point distance strategy, defaults to haversine

\qbk{
[heading See also]
[link geometry.reference.algorithms.distance.distance_3_with_strategy distance (with strategy)]
}

*/
template
<
    typename Point,
    typename PointOfSegment = Point,
    typename CalculationType = void,
    typename Strategy = typename services::default_strategy<point_tag, Point>::type
>
class cross_track
{
public :
    typedef typename promote_floating_point
        <
            typename select_calculation_type
                <
                    Point,
                    PointOfSegment,
                    CalculationType
                >::type
        >::type return_type;

    inline cross_track()
    {
        m_strategy = Strategy();
        m_radius = m_strategy.radius();
    }

    inline cross_track(return_type const& r)
        : m_radius(r)
        , m_strategy(r)
    {}

    inline cross_track(Strategy const& s)
        : m_strategy(s)
    {
        m_radius = m_strategy.radius();
    }


    // It might be useful in the future
    // to overload constructor with strategy info.
    // crosstrack(...) {}


    inline return_type apply(Point const& p,
                PointOfSegment const& sp1, PointOfSegment const& sp2) const
    {
        // http://williams.best.vwh.net/avform.htm#XTE
        return_type d1 = m_strategy.apply(sp1, p);

        // Actually, calculation of d2 not necessary if we know that the projected point is on the great circle...
        return_type d2 = m_strategy.apply(sp2, p);

        return_type crs_AD = course(sp1, p);
        return_type crs_AB = course(sp1, sp2);
        return_type XTD = m_radius * geometry::math::abs(asin(sin(d1 / m_radius) * sin(crs_AD - crs_AB)));

#ifdef BOOST_GEOMETRY_DEBUG_CROSS_TRACK
std::cout << "Course " << dsv(sp1) << " to " << dsv(p) << " " << crs_AD * geometry::math::r2d << std::endl;
std::cout << "Course " << dsv(sp1) << " to " << dsv(sp2) << " " << crs_AB * geometry::math::r2d << std::endl;
std::cout << "XTD: " << XTD << " d1: " <<  d1  << " d2: " <<  d2  << std::endl;
#endif


        // Return shortest distance, either to projected point on segment sp1-sp2, or to sp1, or to sp2
        return return_type((std::min)((std::min)(d1, d2), XTD));
    }

    inline return_type radius() const { return m_radius; }

private :
    BOOST_CONCEPT_ASSERT
        (
            (geometry::concept::PointDistanceStrategy<Strategy >)
        );


    return_type m_radius;

    // Point-point distances are calculated in radians, on the unit sphere
    Strategy m_strategy;

    /// Calculate course (bearing) between two points. Might be moved to a "course formula" ...
    inline return_type course(Point const& p1, Point const& p2) const
    {
        // http://williams.best.vwh.net/avform.htm#Crs
        return_type dlon = get_as_radian<0>(p2) - get_as_radian<0>(p1);
        return_type cos_p2lat = cos(get_as_radian<1>(p2));

        // "An alternative formula, not requiring the pre-computation of d"
        return atan2(sin(dlon) * cos_p2lat,
            cos(get_as_radian<1>(p1)) * sin(get_as_radian<1>(p2))
            - sin(get_as_radian<1>(p1)) * cos_p2lat * cos(dlon));
    }

};



#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
namespace services
{

template <typename Point, typename PointOfSegment, typename CalculationType, typename Strategy>
struct tag<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
    typedef strategy_tag_distance_point_segment type;
};


template <typename Point, typename PointOfSegment, typename CalculationType, typename Strategy>
struct return_type<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
    typedef typename cross_track<Point, PointOfSegment, CalculationType, Strategy>::return_type type;
};


template
<
    typename Point,
    typename PointOfSegment,
    typename CalculationType,
    typename Strategy,
    typename P,
    typename PS
>
struct similar_type<cross_track<Point, PointOfSegment, CalculationType, Strategy>, P, PS>
{
    typedef cross_track<Point, PointOfSegment, CalculationType, Strategy> type;
};


template
<
    typename Point,
    typename PointOfSegment,
    typename CalculationType,
    typename Strategy,
    typename P,
    typename PS
>
struct get_similar<cross_track<Point, PointOfSegment, CalculationType, Strategy>, P, PS>
{
    static inline typename similar_type
        <
            cross_track<Point, PointOfSegment, CalculationType, Strategy>, P, PS
        >::type apply(cross_track<Point, PointOfSegment, CalculationType, Strategy> const& strategy)
    {
        return cross_track<P, PS, CalculationType, Strategy>(strategy.radius());
    }
};


template <typename Point, typename PointOfSegment, typename CalculationType, typename Strategy>
struct comparable_type<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
    // Comparable type is here just the strategy
    typedef typename similar_type
        <
            cross_track
                <
                    Point, PointOfSegment, CalculationType, Strategy
                >, Point, PointOfSegment
        >::type type;
};


template 
<
    typename Point, typename PointOfSegment, 
    typename CalculationType, 
    typename Strategy
>
struct get_comparable<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
    typedef typename comparable_type
        <
            cross_track<Point, PointOfSegment, CalculationType, Strategy>
        >::type comparable_type;
public :
    static inline comparable_type apply(cross_track<Point, PointOfSegment, CalculationType, Strategy> const& strategy)
    {
        return comparable_type(strategy.radius());
    }
};


template 
<
    typename Point, typename PointOfSegment, 
    typename CalculationType, 
    typename Strategy
>
struct result_from_distance<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
private :
    typedef typename cross_track<Point, PointOfSegment, CalculationType, Strategy>::return_type return_type;
public :
    template <typename T>
    static inline return_type apply(cross_track<Point, PointOfSegment, CalculationType, Strategy> const& , T const& distance)
    {
        return distance;
    }
};


template 
<
    typename Point, typename PointOfSegment, 
    typename CalculationType, 
    typename Strategy
>
struct strategy_point_point<cross_track<Point, PointOfSegment, CalculationType, Strategy> >
{
    typedef Strategy type;
};



/*

TODO:  spherical polar coordinate system requires "get_as_radian_equatorial<>"

template <typename Point, typename PointOfSegment, typename Strategy>
struct default_strategy
    <
        segment_tag, Point, PointOfSegment, 
        spherical_polar_tag, spherical_polar_tag, 
        Strategy
    >
{
    typedef cross_track
        <
            Point,
            PointOfSegment,
            void,
            typename boost::mpl::if_
                <
                    boost::is_void<Strategy>,
                    typename default_strategy
                        <
                            point_tag, Point, PointOfSegment,
                            spherical_polar_tag, spherical_polar_tag
                        >::type,
                    Strategy
                >::type
        > type;
};
*/

template <typename Point, typename PointOfSegment, typename Strategy>
struct default_strategy
    <
        segment_tag, Point, PointOfSegment, 
        spherical_equatorial_tag, spherical_equatorial_tag, 
        Strategy
    >
{
    typedef cross_track
        <
            Point,
            PointOfSegment,
            void,
            typename boost::mpl::if_
                <
                    boost::is_void<Strategy>,
                    typename default_strategy
                        <
                            point_tag, Point, PointOfSegment,
                            spherical_equatorial_tag, spherical_equatorial_tag
                        >::type,
                    Strategy
                >::type
        > type;
};



} // namespace services
#endif // DOXYGEN_NO_STRATEGY_SPECIALIZATIONS


}} // namespace strategy::distance


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS


#endif


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_SPHERICAL_DISTANCE_CROSS_TRACK_HPP
