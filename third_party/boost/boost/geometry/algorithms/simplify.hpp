// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP
#define BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP


#include <cstddef>

#include "third_party/boost/boost/range.hpp"
#include "third_party/boost/boost/typeof/typeof.hpp"

#include "third_party/boost/boost/geometry/core/cs.hpp"
#include "third_party/boost/boost/geometry/core/closure.hpp"
#include "third_party/boost/boost/geometry/core/ring_type.hpp"
#include "third_party/boost/boost/geometry/core/exterior_ring.hpp"
#include "third_party/boost/boost/geometry/core/interior_rings.hpp"
#include "third_party/boost/boost/geometry/core/mutable_range.hpp"

#include "third_party/boost/boost/geometry/geometries/concepts/check.hpp"
#include "third_party/boost/boost/geometry/strategies/agnostic/simplify_douglas_peucker.hpp"
#include "third_party/boost/boost/geometry/strategies/concepts/simplify_concept.hpp"

#include "third_party/boost/boost/geometry/algorithms/clear.hpp"
#include "third_party/boost/boost/geometry/algorithms/convert.hpp"
#include "third_party/boost/boost/geometry/algorithms/num_interior_rings.hpp"


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace simplify
{

template<typename Range, typename Strategy>
struct simplify_range_insert
{
    template <typename OutputIterator, typename Distance>
    static inline void apply(Range const& range, OutputIterator out,
                    Distance const& max_distance, Strategy const& strategy)
    {
        if (boost::size(range) <= 2 || max_distance < 0)
        {
            std::copy(boost::begin(range), boost::end(range), out);
        }
        else
        {
            strategy.apply(range, out, max_distance);
        }
    }
};


template<typename Range, typename Strategy>
struct simplify_copy
{
    template <typename Distance>
    static inline void apply(Range const& range, Range& out,
                    Distance const& , Strategy const& )
    {
        std::copy
            (
                boost::begin(range), boost::end(range), std::back_inserter(out)
            );
    }
};


template<typename Range, typename Strategy, std::size_t Minimum>
struct simplify_range
{
    template <typename Distance>
    static inline void apply(Range const& range, Range& out,
                    Distance const& max_distance, Strategy const& strategy)
    {
        // Call do_container for a linestring / ring

        /* For a RING:
            The first/last point (the closing point of the ring) should maybe
            be excluded because it lies on a line with second/one but last.
            Here it is never excluded.

            Note also that, especially if max_distance is too large,
            the output ring might be self intersecting while the input ring is
            not, although chances are low in normal polygons

            Finally the inputring might have 3 (open) or 4 (closed) points (=correct),
                the output < 3 or 4(=wrong)
        */

        if (boost::size(range) <= int(Minimum) || max_distance < 0.0)
        {
            simplify_copy<Range, Strategy>::apply
                (
                    range, out, max_distance, strategy
                );
        }
        else
        {
            simplify_range_insert<Range, Strategy>::apply
                (
                    range, std::back_inserter(out), max_distance, strategy
                );
        }
    }
};

template<typename Polygon, typename Strategy>
struct simplify_polygon
{
    template <typename Distance>
    static inline void apply(Polygon const& poly_in, Polygon& poly_out,
                    Distance const& max_distance, Strategy const& strategy)
    {
        typedef typename ring_type<Polygon>::type ring_type;

        int const Minimum = core_detail::closure::minimum_ring_size
            <
                geometry::closure<Polygon>::value
            >::value;

        // Note that if there are inner rings, and distance is too large,
        // they might intersect with the outer ring in the output,
        // while it didn't in the input.
        simplify_range<ring_type, Strategy, Minimum>::apply(exterior_ring(poly_in),
                        exterior_ring(poly_out),
                        max_distance, strategy);

        traits::resize
            <
                typename boost::remove_reference
                <
                    typename traits::interior_mutable_type<Polygon>::type
                >::type
            >::apply(interior_rings(poly_out), num_interior_rings(poly_in));

        typename interior_return_type<Polygon const>::type rings_in
                    = interior_rings(poly_in);
        typename interior_return_type<Polygon>::type rings_out
                    = interior_rings(poly_out);
        BOOST_AUTO_TPL(it_out, boost::begin(rings_out));
        for (BOOST_AUTO_TPL(it_in,  boost::begin(rings_in));
            it_in != boost::end(rings_in);
            ++it_in, ++it_out)
        {
            simplify_range<ring_type, Strategy, Minimum>::apply(*it_in,
                        *it_out, max_distance, strategy);
        }
    }
};


}} // namespace detail::simplify
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename Tag, typename Geometry, typename Strategy>
struct simplify
{
};

template <typename Point, typename Strategy>
struct simplify<point_tag, Point, Strategy>
{
    template <typename Distance>
    static inline void apply(Point const& point, Point& out,
                    Distance const& , Strategy const& )
    {
        geometry::convert(point, out);
    }
};


template <typename Linestring, typename Strategy>
struct simplify<linestring_tag, Linestring, Strategy>
    : detail::simplify::simplify_range
            <
                Linestring,
                Strategy,
                2
            >
{};

template <typename Ring, typename Strategy>
struct simplify<ring_tag, Ring, Strategy>
    : detail::simplify::simplify_range
            <
                Ring,
                Strategy,
                core_detail::closure::minimum_ring_size
                    <
                        geometry::closure<Ring>::value
                    >::value
            >
{};

template <typename Polygon, typename Strategy>
struct simplify<polygon_tag, Polygon, Strategy>
    : detail::simplify::simplify_polygon
            <
                Polygon,
                Strategy
            >
{};


template <typename Tag, typename Geometry, typename Strategy>
struct simplify_insert
{
};


template <typename Linestring, typename Strategy>
struct simplify_insert<linestring_tag, Linestring, Strategy>
    : detail::simplify::simplify_range_insert
            <
                Linestring,
                Strategy
            >
{};

template <typename Ring, typename Strategy>
struct simplify_insert<ring_tag, Ring, Strategy>
    : detail::simplify::simplify_range_insert
            <
                Ring,
                Strategy
            >
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
\brief Simplify a geometry using a specified strategy
\ingroup simplify
\tparam Geometry \tparam_geometry
\tparam Distance A numerical distance measure
\tparam Strategy A type fulfilling a SimplifyStrategy concept
\param strategy A strategy to calculate simplification
\param geometry input geometry, to be simplified
\param out output geometry, simplified version of the input geometry
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed
\param strategy simplify strategy to be used for simplification, might
    include point-distance strategy

\image html svg_simplify_country.png "The image below presents the simplified country"
\qbk{distinguish,with strategy}
*/
template<typename Geometry, typename Distance, typename Strategy>
inline void simplify(Geometry const& geometry, Geometry& out,
                     Distance const& max_distance, Strategy const& strategy)
{
    concept::check<Geometry>();

    BOOST_CONCEPT_ASSERT( (geometry::concept::SimplifyStrategy<Strategy>) );

    geometry::clear(out);

    dispatch::simplify
        <
            typename tag<Geometry>::type,
            Geometry,
            Strategy
        >::apply(geometry, out, max_distance, strategy);
}




/*!
\brief Simplify a geometry
\ingroup simplify
\tparam Geometry \tparam_geometry
\tparam Distance \tparam_numeric
\note This version of simplify simplifies a geometry using the default
    strategy (Douglas Peucker),
\param geometry input geometry, to be simplified
\param out output geometry, simplified version of the input geometry
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed

\qbk{[include reference/algorithms/simplify.qbk]}
 */
template<typename Geometry, typename Distance>
inline void simplify(Geometry const& geometry, Geometry& out,
                     Distance const& max_distance)
{
    concept::check<Geometry>();

    typedef typename point_type<Geometry>::type point_type;
    typedef typename strategy::distance::services::default_strategy
            <
                segment_tag, point_type
            >::type ds_strategy_type;

    typedef strategy::simplify::douglas_peucker
        <
            point_type, ds_strategy_type
        > strategy_type;

    simplify(geometry, out, max_distance, strategy_type());
}


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace simplify
{


/*!
\brief Simplify a geometry, using an output iterator
    and a specified strategy
\ingroup simplify
\tparam Geometry \tparam_geometry
\param geometry input geometry, to be simplified
\param out output iterator, outputs all simplified points
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed
\param strategy simplify strategy to be used for simplification,
    might include point-distance strategy

\qbk{distinguish,with strategy}
\qbk{[include reference/algorithms/simplify.qbk]}
*/
template<typename Geometry, typename OutputIterator, typename Distance, typename Strategy>
inline void simplify_insert(Geometry const& geometry, OutputIterator out,
                              Distance const& max_distance, Strategy const& strategy)
{
    concept::check<Geometry const>();
    BOOST_CONCEPT_ASSERT( (geometry::concept::SimplifyStrategy<Strategy>) );

    dispatch::simplify_insert
        <
            typename tag<Geometry>::type,
            Geometry,
            Strategy
        >::apply(geometry, out, max_distance, strategy);
}

/*!
\brief Simplify a geometry, using an output iterator
\ingroup simplify
\tparam Geometry \tparam_geometry
\param geometry input geometry, to be simplified
\param out output iterator, outputs all simplified points
\param max_distance distance (in units of input coordinates) of a vertex
    to other segments to be removed

\qbk{[include reference/algorithms/simplify_insert.qbk]}
 */
template<typename Geometry, typename OutputIterator, typename Distance>
inline void simplify_insert(Geometry const& geometry, OutputIterator out,
                              Distance const& max_distance)
{
    typedef typename point_type<Geometry>::type point_type;

    // Concept: output point type = point type of input geometry
    concept::check<Geometry const>();
    concept::check<point_type>();

    typedef typename strategy::distance::services::default_strategy
        <
            segment_tag, point_type
        >::type ds_strategy_type;

    typedef strategy::simplify::douglas_peucker
        <
            point_type, ds_strategy_type
        > strategy_type;

    dispatch::simplify_insert
        <
            typename tag<Geometry>::type,
            Geometry,
            strategy_type
        >::apply(geometry, out, max_distance, strategy_type());
}

}} // namespace detail::simplify
#endif // DOXYGEN_NO_DETAIL



}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_SIMPLIFY_HPP
