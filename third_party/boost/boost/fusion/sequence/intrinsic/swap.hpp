/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_SWAP_20070501_1956)
#define BOOST_FUSION_SWAP_20070501_1956

#include <algorithm>

#include "third_party/boost/boost/fusion/support/is_sequence.hpp"
#include "third_party/boost/boost/fusion/view/zip_view.hpp"
#include "third_party/boost/boost/fusion/algorithm/iteration/for_each.hpp"
#include "third_party/boost/boost/utility/enable_if.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/front.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/back.hpp"
#include "third_party/boost/boost/mpl/and.hpp"

namespace boost { namespace fusion {
    namespace result_of
    {
        template<typename Seq1, typename Seq2>
        struct swap
        {
            typedef void type;
        };
    }

    namespace detail
    {
        struct swap
        {
            template<typename Elem>
            struct result
            {
                typedef void type;
            };

            template<typename Elem>
            void operator()(Elem const& e) const
            {
                using std::swap;
                swap(front(e), back(e));
            }
        };
    }

    template<typename Seq1, typename Seq2>
    typename enable_if<mpl::and_<traits::is_sequence<Seq1>, traits::is_sequence<Seq2> >, void>::type 
    swap(Seq1& lhs, Seq2& rhs)
    {
        typedef vector<Seq1&, Seq2&> references;
        for_each(zip_view<references>(references(lhs, rhs)), detail::swap());
    }
}}

#endif
