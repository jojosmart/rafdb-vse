// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_UNION_HPP
#define BOOST_GEOMETRY_ALGORITHMS_UNION_HPP


#include "third_party/boost/boost/mpl/if.hpp"

#include "third_party/boost/boost/range/metafunctions.hpp"

#include "third_party/boost/boost/geometry/core/is_areal.hpp"
#include "third_party/boost/boost/geometry/core/point_order.hpp"
#include "third_party/boost/boost/geometry/core/reverse_dispatch.hpp"
#include "third_party/boost/boost/geometry/geometries/concepts/check.hpp"
#include "third_party/boost/boost/geometry/algorithms/detail/overlay/overlay.hpp"


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    // tag dispatching:
    typename TagIn1, typename TagIn2, typename TagOut,
    // metafunction finetuning helpers:
    bool Areal1, bool Areal2, bool ArealOut,
    // real types
    typename Geometry1, typename Geometry2,
    bool Reverse1, bool Reverse2, bool ReverseOut,
    typename OutputIterator,
    typename GeometryOut,
    typename Strategy
>
struct union_insert
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_OR_NOT_YET_IMPLEMENTED_FOR_THIS_GEOMETRY_TYPES
            , (types<Geometry1, Geometry2, GeometryOut>)
        );
};


template
<
    typename TagIn1, typename TagIn2, typename TagOut,
    typename Geometry1, typename Geometry2,
    bool Reverse1, bool Reverse2, bool ReverseOut,
    typename OutputIterator,
    typename GeometryOut,
    typename Strategy
>
struct union_insert
    <
        TagIn1, TagIn2, TagOut,
        true, true, true,
        Geometry1, Geometry2,
        Reverse1, Reverse2, ReverseOut,
        OutputIterator, GeometryOut,
        Strategy
    > : detail::overlay::overlay
        <Geometry1, Geometry2, Reverse1, Reverse2, ReverseOut, OutputIterator, GeometryOut, overlay_union, Strategy>
{};



template
<
    typename GeometryTag1, typename GeometryTag2, typename GeometryTag3,
    bool Areal1, bool Areal2, bool ArealOut,
    typename Geometry1, typename Geometry2,
    bool Reverse1, bool Reverse2, bool ReverseOut,
    typename OutputIterator, typename GeometryOut,
    typename Strategy
>
struct union_insert_reversed
{
    static inline OutputIterator apply(Geometry1 const& g1,
            Geometry2 const& g2, OutputIterator out,
            Strategy const& strategy)
    {
        return union_insert
            <
                GeometryTag2, GeometryTag1, GeometryTag3,
                Areal2, Areal1, ArealOut,
                Geometry2, Geometry1,
                Reverse2, Reverse1, ReverseOut,
                OutputIterator, GeometryOut,
                Strategy
            >::apply(g2, g1, out, strategy);
    }
};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace union_
{

template
<
    typename GeometryOut,
    typename Geometry1, typename Geometry2,
    typename OutputIterator,
    typename Strategy
>
inline OutputIterator insert(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            OutputIterator out,
            Strategy const& strategy)
{
    return boost::mpl::if_c
        <
            geometry::reverse_dispatch<Geometry1, Geometry2>::type::value,
            dispatch::union_insert_reversed
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                geometry::is_areal<Geometry1>::value,
                geometry::is_areal<Geometry2>::value,
                geometry::is_areal<GeometryOut>::value,
                Geometry1, Geometry2,
                overlay::do_reverse<geometry::point_order<Geometry1>::value>::value,
                overlay::do_reverse<geometry::point_order<Geometry2>::value>::value,
                overlay::do_reverse<geometry::point_order<GeometryOut>::value>::value,
                OutputIterator, GeometryOut,
                Strategy
            >,
            dispatch::union_insert
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<GeometryOut>::type,
                geometry::is_areal<Geometry1>::value,
                geometry::is_areal<Geometry2>::value,
                geometry::is_areal<GeometryOut>::value,
                Geometry1, Geometry2,
                overlay::do_reverse<geometry::point_order<Geometry1>::value>::value,
                overlay::do_reverse<geometry::point_order<Geometry2>::value>::value,
                overlay::do_reverse<geometry::point_order<GeometryOut>::value>::value,
                OutputIterator, GeometryOut,
                Strategy
            >
        >::type::apply(geometry1, geometry2, out, strategy);
}

/*!
\brief_calc2{union} \brief_strategy
\ingroup union
\details \details_calc2{union_insert, spatial set theoretic union}
    \brief_strategy. details_insert{union}
\tparam GeometryOut output geometry type, must be specified
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam OutputIterator output iterator
\tparam Strategy \tparam_strategy_overlay
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param out \param_out{union}
\param strategy \param_strategy{union}
\return \return_out

\qbk{distinguish,with strategy}
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator,
    typename Strategy
>
inline OutputIterator union_insert(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            OutputIterator out,
            Strategy const& strategy)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();
    concept::check<GeometryOut>();

    return detail::union_::insert<GeometryOut>(geometry1, geometry2, out, strategy);
}

/*!
\brief_calc2{union}
\ingroup union
\details \details_calc2{union_insert, spatial set theoretic union}.
    \details_insert{union}
\tparam GeometryOut output geometry type, must be specified
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam OutputIterator output iterator
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param out \param_out{union}
\return \return_out
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator
>
inline OutputIterator union_insert(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            OutputIterator out)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();
    concept::check<GeometryOut>();

    typedef strategy_intersection
        <
            typename cs_tag<GeometryOut>::type,
            Geometry1,
            Geometry2,
            typename geometry::point_type<GeometryOut>::type
        > strategy;

    return union_insert<GeometryOut>(geometry1, geometry2, out, strategy());
}


}} // namespace detail::union_
#endif // DOXYGEN_NO_DETAIL




/*!
\brief Combines two geometries which each other
\ingroup union
\details \details_calc2{union, spatial set theoretic union}.
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam Collection output collection, either a multi-geometry,
    or a std::vector<Geometry> / std::deque<Geometry> etc
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param output_collection the output collection
\note Called union_ because union is a reserved word.

\qbk{[include reference/algorithms/union.qbk]}
*/
template
<
    typename Geometry1,
    typename Geometry2,
    typename Collection
>
inline void union_(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            Collection& output_collection)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();

    typedef typename boost::range_value<Collection>::type geometry_out;
    concept::check<geometry_out>();

    detail::union_::union_insert<geometry_out>(geometry1, geometry2,
                std::back_inserter(output_collection));
}


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_UNION_HPP
