// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_ASSIGN_VALUES_HPP
#define BOOST_GEOMETRY_ALGORITHMS_ASSIGN_VALUES_HPP


#include <cstddef>

#include "third_party/boost/boost/concept/requires.hpp"
#include "third_party/boost/boost/concept_check.hpp"
#include "third_party/boost/boost/mpl/assert.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/numeric/conversion/bounds.hpp"
#include "third_party/boost/boost/numeric/conversion/cast.hpp"
#include "third_party/boost/boost/type_traits.hpp"

#include "third_party/boost/boost/geometry/arithmetic/arithmetic.hpp"
#include "third_party/boost/boost/geometry/algorithms/append.hpp"
#include "third_party/boost/boost/geometry/algorithms/clear.hpp"
#include "third_party/boost/boost/geometry/core/access.hpp"
#include "third_party/boost/boost/geometry/core/exterior_ring.hpp"
#include "third_party/boost/boost/geometry/core/tags.hpp"

#include "third_party/boost/boost/geometry/geometries/concepts/check.hpp"


#include "third_party/boost/boost/geometry/util/for_each_coordinate.hpp"


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace assign
{


template
<
    typename Box, std::size_t Index,
    std::size_t Dimension, std::size_t DimensionCount
>
struct initialize
{
    typedef typename coordinate_type<Box>::type coordinate_type;

    static inline void apply(Box& box, coordinate_type const& value)
    {
        geometry::set<Index, Dimension>(box, value);
        initialize<Box, Index, Dimension + 1, DimensionCount>::apply(box, value);
    }
};


template <typename Box, std::size_t Index, std::size_t DimensionCount>
struct initialize<Box, Index, DimensionCount, DimensionCount>
{
    typedef typename coordinate_type<Box>::type coordinate_type;

    static inline void apply(Box&,  coordinate_type const& )
    {}
};


template <typename Point>
struct assign_zero_point
{
    static inline void apply(Point& point)
    {
        geometry::assign_value(point, 0);
    }
};


template <typename BoxOrSegment>
struct assign_inverse_box_or_segment
{
    typedef typename point_type<BoxOrSegment>::type point_type;

    static inline void apply(BoxOrSegment& geometry)
    {
        typedef typename coordinate_type<point_type>::type bound_type;

        initialize
            <
                BoxOrSegment, 0, 0, dimension<BoxOrSegment>::type::value
            >::apply(
            geometry, boost::numeric::bounds<bound_type>::highest());
        initialize
            <
                BoxOrSegment, 1, 0, dimension<BoxOrSegment>::type::value
            >::apply(
            geometry, boost::numeric::bounds<bound_type>::lowest());
    }
};


template <typename BoxOrSegment>
struct assign_zero_box_or_segment
{
    static inline void apply(BoxOrSegment& geometry)
    {
        typedef typename coordinate_type<BoxOrSegment>::type coordinate_type;

        initialize
            <
                BoxOrSegment, 0, 0, dimension<BoxOrSegment>::type::value
            >::apply(geometry, coordinate_type());
        initialize
            <
                BoxOrSegment, 1, 0, dimension<BoxOrSegment>::type::value
            >::apply(geometry, coordinate_type());
    }
};


template
<
    std::size_t Corner1, std::size_t Corner2,
    typename Box, typename Point
>
inline void assign_box_2d_corner(Box const& box, Point& point)
{
    // Be sure both are 2-Dimensional
    assert_dimension<Box, 2>();
    assert_dimension<Point, 2>();

    // Copy coordinates
    typedef typename coordinate_type<Point>::type coordinate_type;

    geometry::set<0>(point, boost::numeric_cast<coordinate_type>(get<Corner1, 0>(box)));
    geometry::set<1>(point, boost::numeric_cast<coordinate_type>(get<Corner2, 1>(box)));
}



template
<
    typename Geometry, typename Point,
    std::size_t Index,
    std::size_t Dimension, std::size_t DimensionCount
>
struct assign_point_to_index
{

    static inline void apply(Point const& point, Geometry& geometry)
    {
        geometry::set<Index, Dimension>(geometry, boost::numeric_cast
            <
                typename coordinate_type<Geometry>::type
            >(geometry::get<Dimension>(point)));

        assign_point_to_index
            <
                Geometry, Point, Index, Dimension + 1, DimensionCount
            >::apply(point, geometry);
    }
};

template
<
    typename Geometry, typename Point,
    std::size_t Index,
    std::size_t DimensionCount
>
struct assign_point_to_index
    <
        Geometry, Point,
        Index,
        DimensionCount, DimensionCount
    >
{
    static inline void apply(Point const& , Geometry& )
    {
    }
};


template
<
    typename Geometry, typename Point,
    std::size_t Index,
    std::size_t Dimension, std::size_t DimensionCount
>
struct assign_point_from_index
{

    static inline void apply(Geometry const& geometry, Point& point)
    {
        geometry::set<Dimension>( point, boost::numeric_cast
            <
                typename coordinate_type<Point>::type
            >(geometry::get<Index, Dimension>(geometry)));

        assign_point_from_index
            <
                Geometry, Point, Index, Dimension + 1, DimensionCount
            >::apply(geometry, point);
    }
};

template
<
    typename Geometry, typename Point,
    std::size_t Index,
    std::size_t DimensionCount
>
struct assign_point_from_index
    <
        Geometry, Point,
        Index,
        DimensionCount, DimensionCount
    >
{
    static inline void apply(Geometry const&, Point&)
    {
    }
};


template <typename Geometry>
struct assign_2d_box_or_segment
{
    typedef typename coordinate_type<Geometry>::type coordinate_type;

    // Here we assign 4 coordinates to a box of segment
    // -> Most logical is: x1,y1,x2,y2
    // In case the user reverses x1/x2 or y1/y2, for a box, we could reverse them (THAT IS NOT IMPLEMENTED)

    template <typename Type>
    static inline void apply(Geometry& geometry,
                Type const& x1, Type const& y1, Type const& x2, Type const& y2)
    {
        geometry::set<0, 0>(geometry, boost::numeric_cast<coordinate_type>(x1));
        geometry::set<0, 1>(geometry, boost::numeric_cast<coordinate_type>(y1));
        geometry::set<1, 0>(geometry, boost::numeric_cast<coordinate_type>(x2));
        geometry::set<1, 1>(geometry, boost::numeric_cast<coordinate_type>(y2));
    }
};


}} // namespace detail::assign
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename GeometryTag, typename Geometry, std::size_t DimensionCount>
struct assign
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_OR_NOT_YET_IMPLEMENTED_FOR_THIS_GEOMETRY_TYPE
            , (types<Geometry>)
        );
};

template <typename Point>
struct assign<point_tag, Point, 2>
{
    typedef typename coordinate_type<Point>::type coordinate_type;

    template <typename T>
    static inline void apply(Point& point, T const& c1, T const& c2)
    {
        set<0>(point, boost::numeric_cast<coordinate_type>(c1));
        set<1>(point, boost::numeric_cast<coordinate_type>(c2));
    }
};

template <typename Point>
struct assign<point_tag, Point, 3>
{
    typedef typename coordinate_type<Point>::type coordinate_type;

    template <typename T>
    static inline void apply(Point& point, T const& c1, T const& c2, T const& c3)
    {
        set<0>(point, boost::numeric_cast<coordinate_type>(c1));
        set<1>(point, boost::numeric_cast<coordinate_type>(c2));
        set<2>(point, boost::numeric_cast<coordinate_type>(c3));
    }
};

template <typename Box>
struct assign<box_tag, Box, 2>
    : detail::assign::assign_2d_box_or_segment<Box>
{};

template <typename Segment>
struct assign<segment_tag, Segment, 2>
    : detail::assign::assign_2d_box_or_segment<Segment>
{};



template <typename GeometryTag, typename Geometry>
struct assign_zero {};


template <typename Point>
struct assign_zero<point_tag, Point>
    : detail::assign::assign_zero_point<Point>
{};

template <typename Box>
struct assign_zero<box_tag, Box>
    : detail::assign::assign_zero_box_or_segment<Box>
{};

template <typename Segment>
struct assign_zero<segment_tag, Segment>
    : detail::assign::assign_zero_box_or_segment<Segment>
{};


template <typename GeometryTag, typename Geometry>
struct assign_inverse {};

template <typename Box>
struct assign_inverse<box_tag, Box>
    : detail::assign::assign_inverse_box_or_segment<Box>
{};

template <typename Segment>
struct assign_inverse<segment_tag, Segment>
    : detail::assign::assign_inverse_box_or_segment<Segment>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
\brief Assign two coordinates to a geometry (usually a 2D point)
\ingroup assign
\tparam Geometry \tparam_geometry
\tparam Type \tparam_numeric to specify the coordinates
\param geometry \param_geometry
\param c1 \param_x
\param c2 \param_y

\qbk{distinguish, 2 coordinate values}
\qbk{
[heading Example]
[assign_2d_point] [assign_2d_point_output]

[heading See also]
\* [link geometry.reference.algorithms.make.make_2_2_coordinate_values make]
}
 */
template <typename Geometry, typename Type>
inline void assign_values(Geometry& geometry, Type const& c1, Type const& c2)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            geometry::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2);
}

/*!
\brief Assign three values to a geometry (usually a 3D point)
\ingroup assign
\tparam Geometry \tparam_geometry
\tparam Type \tparam_numeric to specify the coordinates
\param geometry \param_geometry
\param c1 \param_x
\param c2 \param_y
\param c3 \param_z

\qbk{distinguish, 3 coordinate values}
\qbk{
[heading Example]
[assign_3d_point] [assign_3d_point_output]

[heading See also]
\* [link geometry.reference.algorithms.make.make_3_3_coordinate_values make]
}
 */
template <typename Geometry, typename Type>
inline void assign_values(Geometry& geometry,
            Type const& c1, Type const& c2, Type const& c3)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            geometry::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2, c3);
}

/*!
\brief Assign four values to a geometry (usually a box or segment)
\ingroup assign
\tparam Geometry \tparam_geometry
\tparam Type \tparam_numeric to specify the coordinates
\param geometry \param_geometry
\param c1 First coordinate (usually x1)
\param c2 Second coordinate (usually y1)
\param c3 Third coordinate (usually x2)
\param c4 Fourth coordinate (usually y2)

\qbk{distinguish, 4 coordinate values}
 */
template <typename Geometry, typename Type>
inline void assign_values(Geometry& geometry,
                Type const& c1, Type const& c2, Type const& c3, Type const& c4)
{
    concept::check<Geometry>();

    dispatch::assign
        <
            typename tag<Geometry>::type,
            Geometry,
            geometry::dimension<Geometry>::type::value
        >::apply(geometry, c1, c2, c3, c4);
}



}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_ASSIGN_VALUES_HPP
