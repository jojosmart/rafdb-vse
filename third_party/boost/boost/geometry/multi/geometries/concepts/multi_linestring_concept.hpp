// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_LINESTRING_CONCEPT_HPP
#define BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_LINESTRING_CONCEPT_HPP


#include "third_party/boost/boost/concept_check.hpp"
#include "third_party/boost/boost/range/concepts.hpp"
#include "third_party/boost/boost/range/metafunctions.hpp"


#include "third_party/boost/boost/geometry/geometries/concepts/linestring_concept.hpp"


namespace boost { namespace geometry { namespace concept
{


/*!
\brief multi-linestring concept
\ingroup concepts
\par Formal definition:
The multi linestring concept is defined as following:
- there must be a specialization of traits::tag defining multi_linestring_tag as
  type
- it must behave like a Boost.Range
- its range value must fulfil the Linestring concept

*/
template <typename Geometry>
class MultiLinestring
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename boost::range_value<Geometry>::type linestring_type;

    BOOST_CONCEPT_ASSERT( (concept::Linestring<linestring_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(MultiLinestring)
    {
    }
#endif
};


/*!
\brief concept for multi-linestring (const version)
\ingroup const_concepts
*/
template <typename Geometry>
class ConstMultiLinestring
{
#ifndef DOXYGEN_NO_CONCEPT_MEMBERS
    typedef typename boost::range_value<Geometry>::type linestring_type;

    BOOST_CONCEPT_ASSERT( (concept::ConstLinestring<linestring_type>) );
    BOOST_CONCEPT_ASSERT( (boost::RandomAccessRangeConcept<Geometry>) );


public :

    BOOST_CONCEPT_USAGE(ConstMultiLinestring)
    {
    }
#endif
};

}}} // namespace boost::geometry::concept


#endif // BOOST_GEOMETRY_MULTI_GEOMETRIES_CONCEPTS_MULTI_LINESTRING_CONCEPT_HPP
