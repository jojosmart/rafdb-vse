// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2011-2012 Akira Takahashi
// Copyright (c) 2011-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_GEOMETRIES_ADAPTED_FUSION_HPP
#define BOOST_GEOMETRY_GEOMETRIES_ADAPTED_FUSION_HPP


#include <cstddef>

#include "third_party/boost/boost/fusion/include/is_sequence.hpp"
#include "third_party/boost/boost/fusion/include/size.hpp"
#include "third_party/boost/boost/fusion/include/tag_of.hpp"
#include "third_party/boost/boost/fusion/include/front.hpp"
#include "third_party/boost/boost/fusion/include/at.hpp"
#include "third_party/boost/boost/utility/enable_if.hpp"

#include "third_party/boost/boost/fusion/mpl.hpp"
#include "third_party/boost/boost/mpl/front.hpp"
#include "third_party/boost/boost/mpl/count_if.hpp"
#include "third_party/boost/boost/mpl/pop_front.hpp"
#include "third_party/boost/boost/mpl/size.hpp"
#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/type_traits/remove_reference.hpp"
#include "third_party/boost/boost/mpl/placeholders.hpp"
#include "third_party/boost/boost/mpl/and.hpp"
#include "third_party/boost/boost/mpl/front.hpp"

#include "third_party/boost/boost/geometry/core/access.hpp"
#include "third_party/boost/boost/geometry/core/coordinate_dimension.hpp"
#include "third_party/boost/boost/geometry/core/coordinate_system.hpp"
#include "third_party/boost/boost/geometry/core/coordinate_type.hpp"
#include "third_party/boost/boost/geometry/core/point_type.hpp"
#include "third_party/boost/boost/geometry/core/tags.hpp"


namespace boost { namespace geometry
{

namespace fusion_adapt_detail
{

template <class Sequence>
struct all_same :
    boost::mpl::bool_<
        boost::mpl::count_if<
            Sequence,
            boost::is_same<
                typename boost::mpl::front<Sequence>::type,
                boost::mpl::_
            >
        >::value == boost::mpl::size<Sequence>::value
    >
{};

template <class Sequence>
struct is_coordinate_size : boost::mpl::bool_<
            boost::fusion::result_of::size<Sequence>::value == 2 ||
            boost::fusion::result_of::size<Sequence>::value == 3> {};

template<typename Sequence>
struct is_fusion_sequence
    : mpl::and_<boost::fusion::traits::is_sequence<Sequence>,
                fusion_adapt_detail::is_coordinate_size<Sequence>,
                fusion_adapt_detail::all_same<Sequence> >
{};


} // namespace fusion_adapt_detail


#ifndef DOXYGEN_NO_TRAITS_SPECIALIZATIONS
namespace traits
{

// Boost Fusion Sequence, 2D or 3D
template <typename Sequence>
struct coordinate_type
    <
        Sequence,
        typename boost::enable_if
            <
                fusion_adapt_detail::is_fusion_sequence<Sequence>
            >::type
    >
{
    typedef typename boost::mpl::front<Sequence>::type type;
};


template <typename Sequence>
struct dimension
    <
        Sequence,
        typename boost::enable_if
            <
                fusion_adapt_detail::is_fusion_sequence<Sequence>
            >::type
    > : boost::mpl::size<Sequence>
{};


template <typename Sequence, std::size_t Dimension>
struct access
    <
        Sequence,
        Dimension,
        typename boost::enable_if
            <
                fusion_adapt_detail::is_fusion_sequence<Sequence>
            >::type
    >
{
    typedef typename coordinate_type<Sequence>::type ctype;

    static inline ctype get(Sequence const& point)
    {
        return boost::fusion::at_c<Dimension>(point);
    }

    template <class CoordinateType>
    static inline void set(Sequence& point, CoordinateType const& value)
    {
        boost::fusion::at_c<Dimension>(point) = value;
    }
};


template <typename Sequence>
struct tag
    <
        Sequence,
        typename boost::enable_if
            <
                fusion_adapt_detail::is_fusion_sequence<Sequence>
            >::type
    >
{
    typedef point_tag type;
};


} // namespace traits

#endif // DOXYGEN_NO_TRAITS_SPECIALIZATIONS


}} // namespace boost::geometry


// Convenience registration macro to bind a Fusion sequence to a CS
#define BOOST_GEOMETRY_REGISTER_BOOST_FUSION_CS(CoordinateSystem) \
    namespace boost { namespace geometry { namespace traits { \
    template <typename Sequence> \
    struct coordinate_system \
               < \
                   Sequence, \
                   typename boost::enable_if \
                       < \
                           fusion_adapt_detail::is_fusion_sequence<Sequence> \
                       >::type \
               > \
    { typedef CoordinateSystem type; }; \
    }}}


#endif // BOOST_GEOMETRY_GEOMETRIES_ADAPTED_FUSION_HPP
