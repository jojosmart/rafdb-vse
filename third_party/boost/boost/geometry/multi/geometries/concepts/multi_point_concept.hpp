// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_POINT_CONCEPT_HPP
#define BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_POINT_CONCEPT_HPP


#include "third_party/boost/boost/concept_check.hpp"
#include "third_party/boost/boost/range/concepts.hpp"
#include "third_party/boost/boost/range/metafunctions.hpp"


#include "third_party/boost/boost/geometry/geometries/concepts/point_concept.hpp"


namespace boost { namespace geometry { namespace concept
{


/*!
\brief MultiPoint concept
\ingroup concepts
\par Formal definition:
The multi point concept is defined as following:
- there must be a specialization of traits::tag defining multi_point_tag as type
- it must behave like a Boost.Range
- its range value must fulfil the Point concept

*/
template <typename Geometry>
class MultiPoint
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename boost::range_value<Geometry>::type point_type;

    BOOST_CONCEPT_ASSERT( (concept::Point<point_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(MultiPoint)
    {
    }
#endif
};


/*!
\brief concept for multi-point (const version)
\ingroup const_concepts
*/
template <typename Geometry>
class ConstMultiPoint
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename boost::range_value<Geometry>::type point_type;

    BOOST_CONCEPT_ASSERT( (concept::ConstPoint<point_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(ConstMultiPoint)
    {
    }
#endif
};

}}} // namespace boost::geometry::concept


#endif // BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_POINT_CONCEPT_HPP
