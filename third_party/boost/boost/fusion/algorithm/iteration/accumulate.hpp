/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_ACCUMULATE_09172005_1032)
#define FUSION_ACCUMULATE_09172005_1032

#include "third_party/boost/boost/fusion/algorithm/iteration/accumulate_fwd.hpp"
#include "third_party/boost/boost/fusion/algorithm/iteration/fold.hpp"

namespace boost { namespace fusion
{
    struct void_;

    namespace result_of
    {
        template <typename Sequence, typename State, typename F>
        struct accumulate
            : result_of::fold<Sequence, State, F>
        {};
    }

    template <typename Sequence, typename State, typename F>
    inline typename result_of::accumulate<Sequence, State const, F>::type
    accumulate(Sequence& seq, State const& state, F f)
    {
        return fusion::fold(seq, state, f);
    }

    template <typename Sequence, typename State, typename F>
    inline typename result_of::accumulate<Sequence const, State const, F>::type
    accumulate(Sequence const& seq, State const& state, F f)
    {
        return fusion::fold(seq, state, f);
    }
}}

#endif

