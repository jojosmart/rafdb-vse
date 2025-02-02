// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_UTIL_MATH_HPP
#define BOOST_GEOMETRY_UTIL_MATH_HPP

#include <cmath>
#include <limits>

#include "third_party/boost/boost/math/constants/constants.hpp"

#include "third_party/boost/boost/geometry/util/select_most_precise.hpp"

namespace boost { namespace geometry
{

namespace math
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail
{


template <typename Type, bool IsFloatingPoint>
struct equals
{
    static inline bool apply(Type const& a, Type const& b)
    {
        return a == b;
    }
};

template <typename Type>
struct equals<Type, true>
{
    static inline bool apply(Type const& a, Type const& b)
    {
        // See http://www.parashift.com/c++-faq-lite/newbie.html#faq-29.17,
        // FUTURE: replace by some boost tool or boost::test::close_at_tolerance
        return std::abs(a - b) <= std::numeric_limits<Type>::epsilon() * std::abs(a);
    }
};


template <typename Type, bool IsFloatingPoint> 
struct equals_with_epsilon : public equals<Type, IsFloatingPoint> {};


/*!
\brief Short construct to enable partial specialization for PI, currently not possible in Math.
*/
template <typename T>
struct define_pi
{
    static inline T apply()
    {
        // Default calls Boost.Math
        return boost::math::constants::pi<T>();
    }
};


} // namespace detail
#endif


template <typename T>
inline T pi() { return detail::define_pi<T>::apply(); }


// Maybe replace this by boost equals or boost ublas numeric equals or so

/*!
    \brief returns true if both arguments are equal.
    \ingroup utility
    \param a first argument
    \param b second argument
    \return true if a == b
    \note If both a and b are of an integral type, comparison is done by ==.
    If one of the types is floating point, comparison is done by abs and
    comparing with epsilon. If one of the types is non-fundamental, it might
    be a high-precision number and comparison is done using the == operator
    of that class.
*/

template <typename T1, typename T2>
inline bool equals(T1 const& a, T2 const& b)
{
    typedef typename select_most_precise<T1, T2>::type select_type;
    return detail::equals
        <
            select_type,
            boost::is_floating_point<select_type>::type::value
        >::apply(a, b);
}

template <typename T1, typename T2>
inline bool equals_with_epsilon(T1 const& a, T2 const& b)
{
    typedef typename select_most_precise<T1, T2>::type select_type;
    return detail::equals_with_epsilon
        <
            select_type, 
            boost::is_floating_point<select_type>::type::value
        >::apply(a, b);
}



double const d2r = geometry::math::pi<double>() / 180.0;
double const r2d = 1.0 / d2r;

/*!
    \brief Calculates the haversine of an angle
    \ingroup utility
    \note See http://en.wikipedia.org/wiki/Haversine_formula
    haversin(alpha) = sin2(alpha/2)
*/
template <typename T>
inline T hav(T const& theta)
{
    T const half = T(0.5);
    T const sn = sin(half * theta);
    return sn * sn;
}

/*!
\brief Short utility to return the square
\ingroup utility
\param value Value to calculate the square from
\return The squared value
*/
template <typename T>
inline T sqr(T const& value)
{
    return value * value;
}


/*!
\brief Short utility to workaround gcc/clang problem that abs is converting to integer
\ingroup utility
*/
template<typename T>
inline T abs(const T& t)
{
    using std::abs;
    return abs(t);
}


} // namespace math


}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_UTIL_MATH_HPP
