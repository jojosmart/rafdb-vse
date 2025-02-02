// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 1995, 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 1995 Maarten Hilferink, Amsterdam, the Netherlands

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGY_AGNOSTIC_SIMPLIFY_DOUGLAS_PEUCKER_HPP
#define BOOST_GEOMETRY_STRATEGY_AGNOSTIC_SIMPLIFY_DOUGLAS_PEUCKER_HPP


#include <cstddef>
#include <vector>

#include "third_party/boost/boost/range.hpp"

#include "third_party/boost/boost/geometry/core/cs.hpp"
#include "third_party/boost/boost/geometry/strategies/distance.hpp"



//#define GL_DEBUG_DOUGLAS_PEUCKER

#ifdef GL_DEBUG_DOUGLAS_PEUCKER
#include "third_party/boost/boost/geometry/io/dsv/write.hpp"
#endif


namespace boost { namespace geometry
{

namespace strategy { namespace simplify
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail
{

    /*!
        \brief Small wrapper around a point, with an extra member "included"
        \details
            It has a const-reference to the original point (so no copy here)
        \tparam the enclosed point type
    */
    template<typename Point>
    struct douglas_peucker_point
    {
        Point const& p;
        bool included;

        inline douglas_peucker_point(Point const& ap)
            : p(ap)
            , included(false)
        {}

        // Necessary for proper compilation
        inline douglas_peucker_point<Point> operator=(douglas_peucker_point<Point> const& )
        {
            return douglas_peucker_point<Point>(*this);
        }
    };
}
#endif // DOXYGEN_NO_DETAIL


/*!
\brief Implements the simplify algorithm.
\ingroup strategies
\details The douglas_peucker strategy simplifies a linestring, ring or
    vector of points using the well-known Douglas-Peucker algorithm.
    For the algorithm, see for example:
\see http://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm
\see http://www2.dcs.hull.ac.uk/CISRG/projects/Royal-Inst/demos/dp.html
\tparam Point the point type
\tparam PointDistanceStrategy point-segment distance strategy to be used
\note This strategy uses itself a point-segment-distance strategy which
    can be specified
\author Barend and Maarten, 1995/1996
\author Barend, revised for Generic Geometry Library, 2008
*/
template
<
    typename Point,
    typename PointDistanceStrategy
>
class douglas_peucker
{
public :

	// See also ticket 5954 https://svn.boost.org/trac/boost/ticket/5954
	// Comparable is currently not possible here because it has to be compared to the squared of max_distance, and more.
	// For now we have to take the real distance.
	typedef PointDistanceStrategy distance_strategy_type;
    // typedef typename strategy::distance::services::comparable_type<PointDistanceStrategy>::type distance_strategy_type;

    typedef typename strategy::distance::services::return_type<distance_strategy_type>::type return_type;

private :
    typedef detail::douglas_peucker_point<Point> dp_point_type;
    typedef typename std::vector<dp_point_type>::iterator iterator_type;


    static inline void consider(iterator_type begin,
                iterator_type end,
                return_type const& max_dist, int& n,
                distance_strategy_type const& ps_distance_strategy)
    {
        std::size_t size = end - begin;

        // size must be at least 3
        // because we want to consider a candidate point in between
        if (size <= 2)
        {
#ifdef GL_DEBUG_DOUGLAS_PEUCKER
            if (begin != end)
            {
                std::cout << "ignore between " << dsv(begin->p)
                    << " and " << dsv((end - 1)->p)
                    << " size=" << size << std::endl;
            }
            std::cout << "return because size=" << size << std::endl;
#endif
            return;
        }

        iterator_type last = end - 1;

#ifdef GL_DEBUG_DOUGLAS_PEUCKER
        std::cout << "find between " << dsv(begin->p)
            << " and " << dsv(last->p)
            << " size=" << size << std::endl;
#endif


        // Find most far point, compare to the current segment
        //geometry::segment<Point const> s(begin->p, last->p);
        return_type md(-1.0); // any value < 0
        iterator_type candidate;
        for(iterator_type it = begin + 1; it != last; ++it)
        {
            return_type dist = ps_distance_strategy.apply(it->p, begin->p, last->p);

#ifdef GL_DEBUG_DOUGLAS_PEUCKER
            std::cout << "consider " << dsv(it->p)
                << " at " << double(dist)
                << ((dist > max_dist) ? " maybe" : " no") 
                << std::endl;

#endif
            if (dist > md)
            {
                md = dist;
                candidate = it;
            }
        }

        // If a point is found, set the include flag
        // and handle segments in between recursively
        if (md > max_dist)
        {
#ifdef GL_DEBUG_DOUGLAS_PEUCKER
            std::cout << "use " << dsv(candidate->p) << std::endl;
#endif

            candidate->included = true;
            n++;

            consider(begin, candidate + 1, max_dist, n, ps_distance_strategy);
            consider(candidate, end, max_dist, n, ps_distance_strategy);
        }
    }


public :

    template <typename Range, typename OutputIterator>
    static inline OutputIterator apply(Range const& range,
                    OutputIterator out, double max_distance)
    {
        distance_strategy_type strategy;

        // Copy coordinates, a vector of references to all points
        std::vector<dp_point_type> ref_candidates(boost::begin(range),
                        boost::end(range));

        // Include first and last point of line,
        // they are always part of the line
        int n = 2;
        ref_candidates.front().included = true;
        ref_candidates.back().included = true;

        // Get points, recursively, including them if they are further away
        // than the specified distance
        typedef typename strategy::distance::services::return_type<distance_strategy_type>::type return_type;

        consider(boost::begin(ref_candidates), boost::end(ref_candidates), max_distance, n, strategy);

        // Copy included elements to the output
        for(typename std::vector<dp_point_type>::const_iterator it
                        = boost::begin(ref_candidates);
            it != boost::end(ref_candidates);
            ++it)
        {
            if (it->included)
            {
                // copy-coordinates does not work because OutputIterator
                // does not model Point (??)
                //geometry::convert(it->p, *out);
                *out = it->p;
                out++;
            }
        }
        return out;
    }

};

}} // namespace strategy::simplify


}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_STRATEGY_AGNOSTIC_SIMPLIFY_DOUGLAS_PEUCKER_HPP
