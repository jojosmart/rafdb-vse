///////////////////////////////////////////////////////////////////////////////
// weighted_tail_quantile.hpp
//
//  Copyright 2006 Daniel Egloff, Olivier Gygi. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_WEIGHTED_TAIL_QUANTILE_HPP_DE_01_01_2006
#define BOOST_ACCUMULATORS_STATISTICS_WEIGHTED_TAIL_QUANTILE_HPP_DE_01_01_2006

#include <vector>
#include <limits>
#include <functional>
#include <sstream>
#include <stdexcept>
#include "third_party/boost/boost/throw_exception.hpp"
#include "third_party/boost/boost/parameter/keyword.hpp"
#include "third_party/boost/boost/mpl/placeholders.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/accumulators/numeric/functional.hpp"
#include "third_party/boost/boost/accumulators/framework/depends_on.hpp"
#include "third_party/boost/boost/accumulators/framework/accumulator_base.hpp"
#include "third_party/boost/boost/accumulators/framework/extractor.hpp"
#include "third_party/boost/boost/accumulators/framework/parameters/sample.hpp"
#include "third_party/boost/boost/accumulators/statistics_fwd.hpp"
#include "third_party/boost/boost/accumulators/statistics/tail.hpp"
#include "third_party/boost/boost/accumulators/statistics/tail_quantile.hpp"
#include "third_party/boost/boost/accumulators/statistics/parameters/quantile_probability.hpp"

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4127) // conditional expression is constant
#endif

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // weighted_tail_quantile_impl
    //  Tail quantile estimation based on order statistics of weighted samples
    /**
        @brief Tail quantile estimation based on order statistics of weighted samples (for both left and right tails)

        An estimator \f$\hat{q}\f$ of tail quantiles with level \f$\alpha\f$ based on order statistics
        \f$X_{1:n} \leq X_{2:n} \leq\dots\leq X_{n:n}\f$ of weighted samples are given by \f$X_{\lambda:n}\f$ (left tail)
        and \f$X_{\rho:n}\f$ (right tail), where

            \f[
                \lambda = \inf\left\{ l \left| \frac{1}{\bar{w}_n}\sum_{i=1}^{l} w_i \geq \alpha \right. \right\}
            \f]

        and

            \f[
                \rho = \sup\left\{ r \left| \frac{1}{\bar{w}_n}\sum_{i=r}^{n} w_i \geq (1 - \alpha) \right. \right\},
            \f]

        \f$n\f$ being the number of samples and \f$\bar{w}_n\f$ the sum of all weights.

        @param quantile_probability
    */
    template<typename Sample, typename Weight, typename LeftRight>
    struct weighted_tail_quantile_impl
      : accumulator_base
    {
        typedef typename numeric::functional::average<Weight, std::size_t>::result_type float_type;
        // for boost::result_of
        typedef Sample result_type;

        weighted_tail_quantile_impl(dont_care) {}

        template<typename Args>
        result_type result(Args const &args) const
        {
            float_type threshold = sum_of_weights(args)
                             * ( ( is_same<LeftRight, left>::value ) ? args[quantile_probability] : 1. - args[quantile_probability] );

            std::size_t n = 0;
            Weight sum = Weight(0);

            while (sum < threshold)
            {
                if (n < static_cast<std::size_t>(tail_weights(args).size()))
                {
                    sum += *(tail_weights(args).begin() + n);
                    n++;
                }
                else
                {
                    if (std::numeric_limits<result_type>::has_quiet_NaN)
                    {
                        return std::numeric_limits<result_type>::quiet_NaN();
                    }
                    else
                    {
                        std::ostringstream msg;
                        msg << "index n = " << n << " is not in valid range [0, " << tail(args).size() << ")";
                        boost::throw_exception(std::runtime_error(msg.str()));
                        return Sample(0);
                    }
                }
            }

            // Note that the cached samples of the left are sorted in ascending order,
            // whereas the samples of the right tail are sorted in descending order
            return *(boost::begin(tail(args)) + n - 1);
        }
    };
} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::weighted_tail_quantile<>
//
namespace tag
{
    template<typename LeftRight>
    struct weighted_tail_quantile
      : depends_on<sum_of_weights, tail_weights<LeftRight> >
    {
        /// INTERNAL ONLY
        typedef accumulators::impl::weighted_tail_quantile_impl<mpl::_1, mpl::_2, LeftRight> impl;
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::weighted_tail_quantile
//
namespace extract
{
    extractor<tag::quantile> const weighted_tail_quantile = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(weighted_tail_quantile)
}

using extract::weighted_tail_quantile;

}} // namespace boost::accumulators

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#endif
