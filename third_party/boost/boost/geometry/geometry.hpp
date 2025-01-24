// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_GEOMETRY_HPP
#define BOOST_GEOMETRY_GEOMETRY_HPP

// Shortcut to include all header files

#include "third_party/boost/boost/geometry/core/cs.hpp"
#include "third_party/boost/boost/geometry/core/tag.hpp"
#include "third_party/boost/boost/geometry/core/tag_cast.hpp"
#include "third_party/boost/boost/geometry/core/tags.hpp"

// Core algorithms
#include "third_party/boost/boost/geometry/core/access.hpp"
#include "third_party/boost/boost/geometry/core/exterior_ring.hpp"
#include "third_party/boost/boost/geometry/core/interior_rings.hpp"
#include "third_party/boost/boost/geometry/core/radian_access.hpp"
#include "third_party/boost/boost/geometry/core/topological_dimension.hpp"


#include "third_party/boost/boost/geometry/arithmetic/arithmetic.hpp"
#include "third_party/boost/boost/geometry/arithmetic/dot_product.hpp"

#include "third_party/boost/boost/geometry/strategies/strategies.hpp"

#include "third_party/boost/boost/geometry/algorithms/append.hpp"
#include "third_party/boost/boost/geometry/algorithms/area.hpp"
#include "third_party/boost/boost/geometry/algorithms/assign.hpp"
#include "third_party/boost/boost/geometry/algorithms/buffer.hpp"
#include "third_party/boost/boost/geometry/algorithms/centroid.hpp"
#include "third_party/boost/boost/geometry/algorithms/clear.hpp"
#include "third_party/boost/boost/geometry/algorithms/comparable_distance.hpp"
#include "third_party/boost/boost/geometry/algorithms/convert.hpp"
#include "third_party/boost/boost/geometry/algorithms/convex_hull.hpp"
#include "third_party/boost/boost/geometry/algorithms/correct.hpp"
#include "third_party/boost/boost/geometry/algorithms/covered_by.hpp"
#include "third_party/boost/boost/geometry/algorithms/difference.hpp"
#include "third_party/boost/boost/geometry/algorithms/disjoint.hpp"
#include "third_party/boost/boost/geometry/algorithms/distance.hpp"
#include "third_party/boost/boost/geometry/algorithms/envelope.hpp"
#include "third_party/boost/boost/geometry/algorithms/equals.hpp"
#include "third_party/boost/boost/geometry/algorithms/expand.hpp"
#include "third_party/boost/boost/geometry/algorithms/for_each.hpp"
#include "third_party/boost/boost/geometry/algorithms/intersection.hpp"
#include "third_party/boost/boost/geometry/algorithms/intersects.hpp"
#include "third_party/boost/boost/geometry/algorithms/length.hpp"
#include "third_party/boost/boost/geometry/algorithms/make.hpp"
#include "third_party/boost/boost/geometry/algorithms/num_geometries.hpp"
#include "third_party/boost/boost/geometry/algorithms/num_interior_rings.hpp"
#include "third_party/boost/boost/geometry/algorithms/num_points.hpp"
#include "third_party/boost/boost/geometry/algorithms/overlaps.hpp"
#include "third_party/boost/boost/geometry/algorithms/perimeter.hpp"
#include "third_party/boost/boost/geometry/algorithms/reverse.hpp"
#include "third_party/boost/boost/geometry/algorithms/simplify.hpp"
#include "third_party/boost/boost/geometry/algorithms/sym_difference.hpp"
#include "third_party/boost/boost/geometry/algorithms/transform.hpp"
#include "third_party/boost/boost/geometry/algorithms/union.hpp"
#include "third_party/boost/boost/geometry/algorithms/unique.hpp"
#include "third_party/boost/boost/geometry/algorithms/within.hpp"

// Include multi a.o. because it can give weird effects
// if you don't (e.g. area=0 of a multipolygon)
#include "third_party/boost/boost/geometry/multi/multi.hpp"

// check includes all concepts
#include "third_party/boost/boost/geometry/geometries/concepts/check.hpp"

#include "third_party/boost/boost/geometry/util/for_each_coordinate.hpp"
#include "third_party/boost/boost/geometry/util/math.hpp"
#include "third_party/boost/boost/geometry/util/select_most_precise.hpp"
#include "third_party/boost/boost/geometry/util/select_coordinate_type.hpp"
#include "third_party/boost/boost/geometry/io/dsv/write.hpp"

#include "third_party/boost/boost/geometry/views/box_view.hpp"
#include "third_party/boost/boost/geometry/views/segment_view.hpp"

#include "third_party/boost/boost/geometry/io/io.hpp"

#endif // BOOST_GEOMETRY_GEOMETRY_HPP
