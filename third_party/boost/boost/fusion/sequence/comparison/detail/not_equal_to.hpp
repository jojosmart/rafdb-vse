/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_NOT_EQUAL_TO_05052005_1141)
#define FUSION_NOT_EQUAL_TO_05052005_1141

#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/fusion/iterator/deref.hpp"
#include "third_party/boost/boost/fusion/iterator/next.hpp"
#include "third_party/boost/boost/fusion/iterator/equal_to.hpp"

namespace boost { namespace fusion { namespace detail
{
    template <typename Seq1, typename Seq2, bool same_size>
    struct sequence_not_equal_to
    {
        typedef typename result_of::end<Seq1>::type end1_type;
        typedef typename result_of::end<Seq2>::type end2_type;

        template <typename I1, typename I2>
        static bool
        call(I1 const&, I2 const&, mpl::true_)
        {
            return false;
        }

        template <typename I1, typename I2>
        static bool
        call(I1 const& a, I2 const& b, mpl::false_)
        {
            return *a != *b
                || call(fusion::next(a), fusion::next(b));
        }

        template <typename I1, typename I2>
        static bool
        call(I1 const& a, I2 const& b)
        {
            typename result_of::equal_to<I1, end1_type>::type eq;
            return call(a, b, eq);
        }
    };

    template <typename Seq1, typename Seq2>
    struct sequence_not_equal_to<Seq1, Seq2, false>
    {
        template <typename I1, typename I2>
        static bool
        call(I1 const& a, I2 const& b)
        {
            return true;
        }
    };
}}}

#endif
