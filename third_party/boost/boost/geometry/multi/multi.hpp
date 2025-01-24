// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_HPP
#define BOOST_GEOMETRY_MULTI_HPP


#include "third_party/boost/boost/geometry/multi/core/closure.hpp"
#include "third_party/boost/boost/geometry/multi/core/geometry_id.hpp"
#include "third_party/boost/boost/geometry/multi/core/interior_rings.hpp"
#include "third_party/boost/boost/geometry/multi/core/is_areal.hpp"
#include "third_party/boost/boost/geometry/multi/core/point_order.hpp"
#include "third_party/boost/boost/geometry/multi/core/point_type.hpp"
#include "third_party/boost/boost/geometry/multi/core/ring_type.hpp"
#include "third_party/boost/boost/geometry/multi/core/tags.hpp"
#include "third_party/boost/boost/geometry/multi/core/topological_dimension.hpp"

#include "third_party/boost/boost/geometry/multi/algorithms/append.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/area.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/centroid.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/clear.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/convert.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/correct.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/covered_by.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/distance.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/envelope.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/equals.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/for_each.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/intersection.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/length.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/num_geometries.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/num_interior_rings.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/num_points.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/perimeter.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/reverse.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/simplify.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/transform.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/unique.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/within.hpp"

#include "third_party/boost/boost/geometry/multi/algorithms/detail/for_each_range.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/modify_with_predicate.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/multi_sum.hpp"

#include "third_party/boost/boost/geometry/multi/algorithms/detail/sections/range_by_section.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/sections/sectionalize.hpp"

#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/copy_segment_point.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/copy_segments.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/get_ring.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/get_turns.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/select_rings.hpp"
#include "third_party/boost/boost/geometry/multi/algorithms/detail/overlay/self_turn_points.hpp"

#include "third_party/boost/boost/geometry/multi/geometries/concepts/check.hpp"
#include "third_party/boost/boost/geometry/multi/geometries/concepts/multi_point_concept.hpp"
#include "third_party/boost/boost/geometry/multi/geometries/concepts/multi_linestring_concept.hpp"
#include "third_party/boost/boost/geometry/multi/geometries/concepts/multi_polygon_concept.hpp"

#include "third_party/boost/boost/geometry/multi/views/detail/range_type.hpp"
#include "third_party/boost/boost/geometry/multi/strategies/cartesian/centroid_average.hpp"

#include "third_party/boost/boost/geometry/multi/io/dsv/write.hpp"
#include "third_party/boost/boost/geometry/multi/io/wkt/wkt.hpp"


#endif // BOOST_GEOMETRY_MULTI_HPP
