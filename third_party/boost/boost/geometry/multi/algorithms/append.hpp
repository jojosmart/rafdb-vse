// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_ALGORITHMS_APPEND_HPP
#define BOOST_GEOMETRY_MULTI_ALGORITHMS_APPEND_HPP

#include "third_party/boost/boost/geometry/algorithms/append.hpp"

#include "third_party/boost/boost/geometry/multi/core/tags.hpp"


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

namespace splitted_dispatch
{

template <typename Geometry, typename Point>
struct append_point<multi_point_tag, Geometry, Point>
    : detail::append::append_point<Geometry, Point>
{};

template <typename Geometry, typename Range>
struct append_range<multi_point_tag, Geometry, Range>
    : detail::append::append_range<Geometry, Range>
{};

}


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_ALGORITHMS_APPEND_HPP
