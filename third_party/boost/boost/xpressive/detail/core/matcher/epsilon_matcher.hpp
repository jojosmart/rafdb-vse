///////////////////////////////////////////////////////////////////////////////
// epsilon_matcher.hpp
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include "third_party/boost/boost/xpressive/detail/detail_fwd.hpp"
#include "third_party/boost/boost/xpressive/detail/core/quant_style.hpp"
#include "third_party/boost/boost/xpressive/detail/core/state.hpp"

namespace boost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // epsilon_matcher
    //
    struct epsilon_matcher
    {
        BOOST_XPR_QUANT_STYLE(quant_none, 0, true)

        template<typename BidiIter, typename Next>
        static bool match(match_state<BidiIter> &state, Next const &next)
        {
            return next.match(state);
        }
    };

}}}

#endif
