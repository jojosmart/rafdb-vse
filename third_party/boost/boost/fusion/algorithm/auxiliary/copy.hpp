/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_COPY_02162011_2308)
#define FUSION_COPY_02162011_2308

#include "third_party/boost/boost/fusion/sequence/intrinsic/begin.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/end.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/size.hpp"
#include "third_party/boost/boost/fusion/sequence/comparison/detail/equal_to.hpp"
#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/static_assert.hpp"

#if defined (BOOST_MSVC)
#  pragma warning(push)
#  pragma warning (disable: 4100) // unreferenced formal parameter
#endif

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Seq1, typename Seq2>
        struct sequence_copy
        {
            typedef typename result_of::end<Seq1>::type end1_type;
            typedef typename result_of::end<Seq2>::type end2_type;

            template <typename I1, typename I2>
            static void
            call(I1 const&, I2 const&, mpl::true_)
            {
            }

            template <typename I1, typename I2>
            static void
            call(I1 const& src, I2 const& dest, mpl::false_)
            {
                *dest = *src;
                call(fusion::next(src), fusion::next(dest));
            }

            template <typename I1, typename I2>
            static void
            call(I1 const& src, I2 const& dest)
            {
                typename result_of::equal_to<I1, end1_type>::type eq;
                return call(src, dest, eq);
            }
        };
    }

    template <typename Seq1, typename Seq2>
    inline void
    copy(Seq1 const& src, Seq2& dest)
    {
        BOOST_STATIC_ASSERT(
            result_of::size<Seq1>::value == result_of::size<Seq2>::value);

        detail::sequence_copy<
            Seq1 const, Seq2>::
            call(fusion::begin(src), fusion::begin(dest));
    }
}}

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif
