//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATS_EXTREME_VALUE_HPP
#define BOOST_STATS_EXTREME_VALUE_HPP

#include "third_party/boost/boost/math/distributions/fwd.hpp"
#include "third_party/boost/boost/math/constants/constants.hpp"
#include "third_party/boost/boost/math/special_functions/log1p.hpp"
#include "third_party/boost/boost/math/special_functions/expm1.hpp"
#include "third_party/boost/boost/math/distributions/complement.hpp"
#include "third_party/boost/boost/math/distributions/detail/common_error_handling.hpp"
#include "third_party/boost/boost/config/no_tr1/cmath.hpp"

//
// This is the maximum extreme value distribution, see
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda366g.htm
// and http://mathworld.wolfram.com/ExtremeValueDistribution.html
// Also known as a Fisher-Tippett distribution, a log-Weibull
// distribution or a Gumbel distribution.

#include <utility>

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4702) // unreachable code (return after domain_error throw).
#endif

namespace boost{ namespace math{

namespace detail{
//
// Error check:
//
template <class RealType, class Policy>
inline bool verify_scale_b(const char* function, RealType b, RealType* presult, const Policy& pol)
{
   if(b <= 0)
   {
      *presult = policies::raise_domain_error<RealType>(
         function,
         "The scale parameter \"b\" must be > 0, but was: %1%.", b, pol);
      return false;
   }
   return true;
}

} // namespace detail

template <class RealType = double, class Policy = policies::policy<> >
class extreme_value_distribution
{
public:
   typedef RealType value_type;
   typedef Policy policy_type;

   extreme_value_distribution(RealType a = 0, RealType b = 1)
      : m_a(a), m_b(b)
   {
      RealType err;
      detail::verify_scale_b("boost::math::extreme_value_distribution<%1%>::extreme_value_distribution", b, &err, Policy());
   } // extreme_value_distribution

   RealType location()const { return m_a; }
   RealType scale()const { return m_b; }

private:
   RealType m_a, m_b;
};

typedef extreme_value_distribution<double> extreme_value;

template <class RealType, class Policy>
inline const std::pair<RealType, RealType> range(const extreme_value_distribution<RealType, Policy>& /*dist*/)
{ // Range of permissible values for random variable x.
   using boost::math::tools::max_value;
   return std::pair<RealType, RealType>(-max_value<RealType>(), max_value<RealType>());
}

template <class RealType, class Policy>
inline const std::pair<RealType, RealType> support(const extreme_value_distribution<RealType, Policy>& /*dist*/)
{ // Range of supported values for random variable x.
   // This is range where cdf rises from 0 to 1, and outside it, the pdf is zero.
   using boost::math::tools::max_value;
   return std::pair<RealType, RealType>(-max_value<RealType>(),  max_value<RealType>());
}

template <class RealType, class Policy>
inline RealType pdf(const extreme_value_distribution<RealType, Policy>& dist, const RealType& x)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b("boost::math::pdf(const extreme_value_distribution<%1%>&, %1%)", b, &result, Policy()))
      return result;
   result = exp((a-x)/b) * exp(-exp((a-x)/b)) / b;
   return result;
} // pdf

template <class RealType, class Policy>
inline RealType cdf(const extreme_value_distribution<RealType, Policy>& dist, const RealType& x)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b("boost::math::cdf(const extreme_value_distribution<%1%>&, %1%)", b, &result, Policy()))
      return result;

   result = exp(-exp((a-x)/b));

   return result;
} // cdf

template <class RealType, class Policy>
RealType quantile(const extreme_value_distribution<RealType, Policy>& dist, const RealType& p)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::quantile(const extreme_value_distribution<%1%>&, %1%)";

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b(function, b, &result, Policy()))
      return result;
   if(0 == detail::check_probability(function, p, &result, Policy()))
      return result;

   if(p == 0)
      return -policies::raise_overflow_error<RealType>(function, 0, Policy());
   if(p == 1)
      return policies::raise_overflow_error<RealType>(function, 0, Policy());

   result = a - log(-log(p)) * b;

   return result;
} // quantile

template <class RealType, class Policy>
inline RealType cdf(const complemented2_type<extreme_value_distribution<RealType, Policy>, RealType>& c)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   RealType a = c.dist.location();
   RealType b = c.dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b("boost::math::cdf(const extreme_value_distribution<%1%>&, %1%)", b, &result, Policy()))
      return result;

   result = -boost::math::expm1(-exp((a-c.param)/b), Policy());

   return result;
}

template <class RealType, class Policy>
RealType quantile(const complemented2_type<extreme_value_distribution<RealType, Policy>, RealType>& c)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::quantile(const extreme_value_distribution<%1%>&, %1%)";

   RealType a = c.dist.location();
   RealType b = c.dist.scale();
   RealType q = c.param;
   RealType result = 0;
   if(0 == detail::verify_scale_b(function, b, &result, Policy()))
      return result;
   if(0 == detail::check_probability(function, q, &result, Policy()))
      return result;

   if(q == 0)
      return policies::raise_overflow_error<RealType>(function, 0, Policy());
   if(q == 1)
      return -policies::raise_overflow_error<RealType>(function, 0, Policy());

   result = a - log(-boost::math::log1p(-q, Policy())) * b;

   return result;
}

template <class RealType, class Policy>
inline RealType mean(const extreme_value_distribution<RealType, Policy>& dist)
{
   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b("boost::math::mean(const extreme_value_distribution<%1%>&)", b, &result, Policy()))
      return result;
   return a + constants::euler<RealType>() * b;
}

template <class RealType, class Policy>
inline RealType standard_deviation(const extreme_value_distribution<RealType, Policy>& dist)
{
   BOOST_MATH_STD_USING // for ADL of std functions.

   RealType b = dist.scale();
   RealType result = 0;
   if(0 == detail::verify_scale_b("boost::math::standard_deviation(const extreme_value_distribution<%1%>&)", b, &result, Policy()))
      return result;
   return constants::pi<RealType>() * b / sqrt(static_cast<RealType>(6));
}

template <class RealType, class Policy>
inline RealType mode(const extreme_value_distribution<RealType, Policy>& dist)
{
   return dist.location();
}

template <class RealType, class Policy>
inline RealType median(const extreme_value_distribution<RealType, Policy>& dist)
{
  using constants::ln_ln_two;
   return dist.location() - dist.scale() * ln_ln_two<RealType>();
}

template <class RealType, class Policy>
inline RealType skewness(const extreme_value_distribution<RealType, Policy>& /*dist*/)
{
   //
   // This is 12 * sqrt(6) * zeta(3) / pi^3:
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   //
   return static_cast<RealType>(1.1395470994046486574927930193898461120875997958366L);
}

template <class RealType, class Policy>
inline RealType kurtosis(const extreme_value_distribution<RealType, Policy>& /*dist*/)
{
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   return RealType(27) / 5;
}

template <class RealType, class Policy>
inline RealType kurtosis_excess(const extreme_value_distribution<RealType, Policy>& /*dist*/)
{
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   return RealType(12) / 5;
}


} // namespace math
} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

// This include must be at the end, *after* the accessors
// for this distribution have been defined, in order to
// keep compilers that support two-phase lookup happy.
#include "third_party/boost/boost/math/distributions/detail/derived_accessors.hpp"

#endif // BOOST_STATS_EXTREME_VALUE_HPP
