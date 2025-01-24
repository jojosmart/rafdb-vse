// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_STRATEGIES_HPP
#define BOOST_GEOMETRY_STRATEGIES_STRATEGIES_HPP


#include "third_party/boost/boost/geometry/strategies/tags.hpp"

#include "third_party/boost/boost/geometry/strategies/area.hpp"
#include "third_party/boost/boost/geometry/strategies/centroid.hpp"
#include "third_party/boost/boost/geometry/strategies/compare.hpp"
#include "third_party/boost/boost/geometry/strategies/convex_hull.hpp"
#include "third_party/boost/boost/geometry/strategies/distance.hpp"
#include "third_party/boost/boost/geometry/strategies/intersection.hpp"
#include "third_party/boost/boost/geometry/strategies/side.hpp"
#include "third_party/boost/boost/geometry/strategies/transform.hpp"
#include "third_party/boost/boost/geometry/strategies/within.hpp"

#include "third_party/boost/boost/geometry/strategies/cartesian/area_surveyor.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/box_in_box.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/centroid_bashein_detmer.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/centroid_weighted_length.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/distance_pythagoras.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/distance_projected_point.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/point_in_box.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp"
#include "third_party/boost/boost/geometry/strategies/cartesian/side_by_triangle.hpp"

#include "third_party/boost/boost/geometry/strategies/spherical/area_huiller.hpp"
#include "third_party/boost/boost/geometry/strategies/spherical/distance_haversine.hpp"
#include "third_party/boost/boost/geometry/strategies/spherical/distance_cross_track.hpp"
#include "third_party/boost/boost/geometry/strategies/spherical/compare_circular.hpp"
#include "third_party/boost/boost/geometry/strategies/spherical/ssf.hpp"

#include "third_party/boost/boost/geometry/strategies/agnostic/hull_graham_andrew.hpp"
#include "third_party/boost/boost/geometry/strategies/agnostic/point_in_box_by_side.hpp"
#include "third_party/boost/boost/geometry/strategies/agnostic/point_in_poly_winding.hpp"
#include "third_party/boost/boost/geometry/strategies/agnostic/simplify_douglas_peucker.hpp"

#include "third_party/boost/boost/geometry/strategies/strategy_transform.hpp"

#include "third_party/boost/boost/geometry/strategies/transform/matrix_transformers.hpp"
#include "third_party/boost/boost/geometry/strategies/transform/map_transformer.hpp"
#include "third_party/boost/boost/geometry/strategies/transform/inverse_transformer.hpp"


#endif // BOOST_GEOMETRY_STRATEGIES_STRATEGIES_HPP
