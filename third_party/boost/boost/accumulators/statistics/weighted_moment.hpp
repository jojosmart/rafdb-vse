///////////////////////////////////////////////////////////////////////////////
// weighted_moment.hpp
//
//  Copyright 2006, Eric Niebler, Olivier Gygi. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_WEIGHTED_MOMENT_HPP_EAN_15_11_2005
#define BOOST_ACCUMULATORS_STATISTICS_WEIGHTED_MOMENT_HPP_EAN_15_11_2005

#include "third_party/boost/boost/config/no_tr1/cmath.hpp"
#include "third_party/boost/boost/mpl/int.hpp"
#include "third_party/boost/boost/mpl/assert.hpp"
#include "third_party/boost/boost/mpl/placeholders.hpp"
#include "third_party/boost/boost/preprocessor/arithmetic/inc.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_binary_params.hpp"
#include "third_party/boost/boost/accumulators/framework/accumulator_base.hpp"
#include "third_party/boost/boost/accumulators/framework/extractor.hpp"
#include "third_party/boost/boost/accumulators/numeric/functional.hpp"
#include "third_party/boost/boost/accumulators/framework/parameters/sample.hpp"
#include "third_party/boost/boost/accumulators/framework/depends_on.hpp"
#include "third_party/boost/boost/accumulators/statistics_fwd.hpp"
#include "third_party/boost/boost/accumulators/statistics/count.hpp"
#include "third_party/boost/boost/accumulators/statistics/moment.hpp" // for pow()
#include "third_party/boost/boost/accumulators/statistics/sum.hpp"

namespace boost { namespace accumulators
{

namespace impl
{
    ///////////////////////////////////////////////////////////////////////////////
    // weighted_moment_impl
    template<typename N, typename Sample, typename Weight>
    struct weighted_moment_impl
      : accumulator_base // TODO: also depends_on sum of powers
    {
        BOOST_MPL_ASSERT_RELATION(N::value, >, 0);
        typedef typename numeric::functional::multiplies<Sample, Weight>::result_type weighted_sample;
        // for boost::result_of
        typedef typename numeric::functional::average<weighted_sample, Weight>::result_type result_type;

        template<typename Args>
        weighted_moment_impl(Args const &args)
          : sum(args[sample | Sample()] * numeric::one<Weight>::value)
        {
        }

        template<typename Args>
        void operator ()(Args const &args)
        {
            this->sum += args[weight] * numeric::pow(args[sample], N());
        }

        template<typename Args>
        result_type result(Args const &args) const
        {
            return numeric::average(this->sum, sum_of_weights(args));
        }

    private:
        weighted_sample sum;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::weighted_moment
//
namespace tag
{
    template<int N>
    struct weighted_moment
      : depends_on<count, sum_of_weights>
    {
        /// INTERNAL ONLY
        ///
        typedef accumulators::impl::weighted_moment_impl<mpl::int_<N>, mpl::_1, mpl::_2> impl;
    };
}

///////////////////////////////////////////////////////////////////////////////
// extract::weighted_moment
//
namespace extract
{
    BOOST_ACCUMULATORS_DEFINE_EXTRACTOR(tag, weighted_moment, (int))
}

using extract::weighted_moment;

}} // namespace boost::accumulators

#endif
