//  (C) Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_SPECIAL_NEXT_HPP
#define BOOST_MATH_SPECIAL_NEXT_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include "third_party/boost/boost/math/policies/error_handling.hpp"
#include "third_party/boost/boost/math/special_functions/fpclassify.hpp"
#include "third_party/boost/boost/math/special_functions/sign.hpp"
#include "third_party/boost/boost/math/special_functions/trunc.hpp"

#ifdef BOOST_MSVC
#include <float.h>
#endif

namespace boost{ namespace math{

namespace detail{

template <class T>
inline T get_smallest_value(mpl::true_ const&)
{
   return std::numeric_limits<T>::denorm_min();
}

template <class T>
inline T get_smallest_value(mpl::false_ const&)
{
   return tools::min_value<T>();
}

template <class T>
inline T get_smallest_value()
{
#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1310)
   return get_smallest_value<T>(mpl::bool_<std::numeric_limits<T>::is_specialized && (std::numeric_limits<T>::has_denorm == 1)>());
#else
   return get_smallest_value<T>(mpl::bool_<std::numeric_limits<T>::is_specialized && (std::numeric_limits<T>::has_denorm == std::denorm_present)>());
#endif
}

}

template <class T, class Policy>
T float_next(const T& val, const Policy& pol)
{
   BOOST_MATH_STD_USING
   int expon;
   static const char* function = "float_next<%1%>(%1%)";

   if(!(boost::math::isfinite)(val))
   {
      if(val < 0)
         return -tools::max_value<T>();
      return policies::raise_domain_error<T>(
         function,
         "Argument must be finite, but got %1%", val, pol);
   }

   if(val >= tools::max_value<T>())
      return policies::raise_overflow_error<T>(function, 0, pol);

   if(val == 0)
      return detail::get_smallest_value<T>();

   if(-0.5f == frexp(val, &expon))
      --expon; // reduce exponent when val is a power of two, and negative.
   T diff = ldexp(T(1), expon - tools::digits<T>());
   if(diff == 0)
      diff = detail::get_smallest_value<T>();
   return val + diff;
}

#ifdef BOOST_MSVC
template <class Policy>
inline double float_next(const double& val, const Policy& pol)
{
   static const char* function = "float_next<%1%>(%1%)";

   if(!(boost::math::isfinite)(val) && (val > 0))
      return policies::raise_domain_error<double>(
         function,
         "Argument must be finite, but got %1%", val, pol);

   if(val >= tools::max_value<double>())
      return policies::raise_overflow_error<double>(function, 0, pol);

   return ::_nextafter(val, tools::max_value<double>());
}
#endif

template <class T>
inline T float_next(const T& val)
{
   return float_next(val, policies::policy<>());
}

template <class T, class Policy>
T float_prior(const T& val, const Policy& pol)
{
   BOOST_MATH_STD_USING
   int expon;
   static const char* function = "float_prior<%1%>(%1%)";

   if(!(boost::math::isfinite)(val))
   {
      if(val > 0)
         return tools::max_value<T>();
      return policies::raise_domain_error<T>(
         function,
         "Argument must be finite, but got %1%", val, pol);
   }

   if(val <= -tools::max_value<T>())
      return -policies::raise_overflow_error<T>(function, 0, pol);

   if(val == 0)
      return -detail::get_smallest_value<T>();

   T remain = frexp(val, &expon);
   if(remain == 0.5)
      --expon; // when val is a power of two we must reduce the exponent
   T diff = ldexp(T(1), expon - tools::digits<T>());
   if(diff == 0)
      diff = detail::get_smallest_value<T>();
   return val - diff;
}

#ifdef BOOST_MSVC
template <class Policy>
inline double float_prior(const double& val, const Policy& pol)
{
   static const char* function = "float_prior<%1%>(%1%)";

   if(!(boost::math::isfinite)(val) && (val < 0))
      return policies::raise_domain_error<double>(
         function,
         "Argument must be finite, but got %1%", val, pol);

   if(val <= -tools::max_value<double>())
      return -policies::raise_overflow_error<double>(function, 0, pol);

   return ::_nextafter(val, -tools::max_value<double>());
}
#endif

template <class T>
inline T float_prior(const T& val)
{
   return float_prior(val, policies::policy<>());
}

template <class T, class Policy>
inline T nextafter(const T& val, const T& direction, const Policy& pol)
{
   return val < direction ? boost::math::float_next(val, pol) : val == direction ? val : boost::math::float_prior(val, pol);
}

template <class T>
inline T nextafter(const T& val, const T& direction)
{
   return nextafter(val, direction, policies::policy<>());
}

template <class T, class Policy>
T float_distance(const T& a, const T& b, const Policy& pol)
{
   BOOST_MATH_STD_USING
   //
   // Error handling:
   //
   static const char* function = "float_distance<%1%>(%1%, %1%)";
   if(!(boost::math::isfinite)(a))
      return policies::raise_domain_error<T>(
         function,
         "Argument a must be finite, but got %1%", a, pol);
   if(!(boost::math::isfinite)(b))
      return policies::raise_domain_error<T>(
         function,
         "Argument b must be finite, but got %1%", b, pol);
   //
   // Special cases:
   //
   if(a > b)
      return -float_distance(b, a);
   if(a == b)
      return 0;
   if(a == 0)
      return 1 + fabs(float_distance(static_cast<T>(boost::math::sign(b) * detail::get_smallest_value<T>()), b, pol));
   if(b == 0)
      return 1 + fabs(float_distance(static_cast<T>(boost::math::sign(a) * detail::get_smallest_value<T>()), a, pol));
   if(boost::math::sign(a) != boost::math::sign(b))
      return 2 + fabs(float_distance(static_cast<T>(boost::math::sign(b) * detail::get_smallest_value<T>()), b, pol))
         + fabs(float_distance(static_cast<T>(boost::math::sign(a) * detail::get_smallest_value<T>()), a, pol));
   //
   // By the time we get here, both a and b must have the same sign, we want
   // b > a and both postive for the following logic:
   //
   if(a < 0)
      return float_distance(static_cast<T>(-b), static_cast<T>(-a));

   BOOST_ASSERT(a >= 0);
   BOOST_ASSERT(b >= a);

   int expon;
   //
   // Note that if a is a denorm then the usual formula fails
   // because we actually have fewer than tools::digits<T>()
   // significant bits in the representation:
   //
   frexp(((boost::math::fpclassify)(a) == FP_SUBNORMAL) ? tools::min_value<T>() : a, &expon);
   T upper = ldexp(T(1), expon);
   T result = 0;
   expon = tools::digits<T>() - expon;
   //
   // If b is greater than upper, then we *must* split the calculation
   // as the size of the ULP changes with each order of magnitude change:
   //
   if(b > upper)
   {
      result = float_distance(upper, b);
   }
   //
   // Use compensated double-double addition to avoid rounding 
   // errors in the subtraction:
   //
   T mb = -(std::min)(upper, b);
   T x = a + mb;
   T z = x - a;
   T y = (a - (x - z)) + (mb - z);
   if(x < 0)
   {
      x = -x;
      y = -y;
   }
   result += ldexp(x, expon) + ldexp(y, expon);
   //
   // Result must be an integer:
   //
   BOOST_ASSERT(result == floor(result));
   return result;
}

template <class T>
T float_distance(const T& a, const T& b)
{
   return boost::math::float_distance(a, b, policies::policy<>());
}

template <class T, class Policy>
T float_advance(T val, int distance, const Policy& pol)
{
   //
   // Error handling:
   //
   static const char* function = "float_advance<%1%>(%1%, int)";
   if(!(boost::math::isfinite)(val))
      return policies::raise_domain_error<T>(
         function,
         "Argument val must be finite, but got %1%", val, pol);

   if(val < 0)
      return -float_advance(-val, -distance, pol);
   if(distance == 0)
      return val;
   if(distance == 1)
      return float_next(val, pol);
   if(distance == -1)
      return float_prior(val, pol);
   BOOST_MATH_STD_USING
   int expon;
   frexp(val, &expon);
   T limit = ldexp((distance < 0 ? T(0.5f) : T(1)), expon);
   if(val <= tools::min_value<T>())
   {
      limit = sign(T(distance)) * tools::min_value<T>();
   }
   T limit_distance = float_distance(val, limit);
   while(fabs(limit_distance) < abs(distance))
   {
      distance -= itrunc(limit_distance);
      val = limit;
      if(distance < 0) 
      {
         limit /= 2;
         expon--;
      }
      else
      {
         limit *= 2;
         expon++;
      }
      limit_distance = float_distance(val, limit);
   }
   if((0.5f == frexp(val, &expon)) && (distance < 0))
      --expon;
   T diff = 0;
   if(val != 0)
      diff = distance * ldexp(T(1), expon - tools::digits<T>());
   if(diff == 0)
      diff = distance * detail::get_smallest_value<T>();
   return val += diff;
}

template <class T>
inline T float_advance(const T& val, int distance)
{
   return boost::math::float_advance(val, distance, policies::policy<>());
}

}} // namespaces

#endif // BOOST_MATH_SPECIAL_NEXT_HPP

