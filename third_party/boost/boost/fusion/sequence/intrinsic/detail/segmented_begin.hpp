/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_SEGMENTED_BEGIN_HPP_INCLUDED)
#define BOOST_FUSION_SEGMENTED_BEGIN_HPP_INCLUDED

#include "third_party/boost/boost/fusion/sequence/intrinsic/detail/segmented_begin_impl.hpp"
#include "third_party/boost/boost/fusion/iterator/segmented_iterator.hpp"
#include "third_party/boost/boost/fusion/view/iterator_range.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/begin.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/end.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/empty.hpp"
#include "third_party/boost/boost/fusion/container/list/cons.hpp"

namespace boost { namespace fusion { namespace detail
{
    //auto segmented_begin( seq )
    //{
    //    return make_segmented_iterator( segmented_begin_impl( seq, nil ) );
    //}

    template <typename Sequence, typename Nil = fusion::nil>
    struct segmented_begin
    {
        typedef
            segmented_iterator<
                typename segmented_begin_impl<Sequence, Nil>::type
            >
        type;

        static type call(Sequence& seq)
        {
            return type(
                segmented_begin_impl<Sequence, Nil>::call(seq, Nil()));
        }
    };

}}}

#endif
