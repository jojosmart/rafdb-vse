/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_COUNT_IF_09162005_0137)
#define BOOST_FUSION_COUNT_IF_09162005_0137

#include "third_party/boost/boost/fusion/algorithm/query/detail/count_if.hpp"
#include "third_party/boost/boost/fusion/support/category_of.hpp"

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename F>
        struct count_if
        {
            typedef int type;
        };
    }

    template <typename Sequence, typename F>
    inline int
    count_if(Sequence const& seq, F f)
    {
        return detail::count_if(
            seq, f, typename traits::category_of<Sequence>::type());
    }
}}

#endif

