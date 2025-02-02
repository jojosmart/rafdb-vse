///////////////////////////////////////////////////////////////////////////////
// rolling_mean.hpp
//
// Copyright 2008 Eric Niebler. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ROLLING_MEAN_HPP_EAN_26_12_2008
#define BOOST_ACCUMULATORS_STATISTICS_ROLLING_MEAN_HPP_EAN_26_12_2008

#include "third_party/boost/boost/mpl/placeholders.hpp"
#include "third_party/boost/boost/accumulators/framework/accumulator_base.hpp"
#include "third_party/boost/boost/accumulators/framework/extractor.hpp"
#include "third_party/boost/boost/accumulators/numeric/functional.hpp"
#include "third_party/boost/boost/accumulators/framework/parameters/sample.hpp"
#include "third_party/boost/boost/accumulators/framework/depends_on.hpp"
#include "third_party/boost/boost/accumulators/statistics_fwd.hpp"
#include "third_party/boost/boost/accumulators/statistics/rolling_sum.hpp"
#include "third_party/boost/boost/accumulators/statistics/rolling_count.hpp"

namespace boost { namespace accumulators
{

namespace impl
{

    ///////////////////////////////////////////////////////////////////////////////
    // rolling_mean_impl
    //    returns the unshifted results from the shifted rolling window
    template<typename Sample>
    struct rolling_mean_impl
      : accumulator_base
    {
        typedef typename numeric::functional::average<Sample, std::size_t>::result_type result_type;

        rolling_mean_impl(dont_care)
        {}

        template<typename Args>
        result_type result(Args const &args) const
        {
            return numeric::average(rolling_sum(args), rolling_count(args));
        }
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::rolling_mean
//
namespace tag
{
    struct rolling_mean
      : depends_on< rolling_sum, rolling_count >
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::rolling_mean_impl< mpl::_1 > impl;

        #ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
        /// tag::rolling_window::window_size named parameter
        static boost::parameter::keyword<tag::rolling_window_size> const window_size;
        #endif
    };
} // namespace tag

///////////////////////////////////////////////////////////////////////////////
// extract::rolling_mean
//
namespace extract
{
    extractor<tag::rolling_mean> const rolling_mean = {};

    BOOST_ACCUMULATORS_IGNORE_GLOBAL(rolling_mean)
}

using extract::rolling_mean;

}} // namespace boost::accumulators

#endif
