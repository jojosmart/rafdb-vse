/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_FILTER_VIEW_ITERATOR_05062005_0849)
#define FUSION_FILTER_VIEW_ITERATOR_05062005_0849

#include "third_party/boost/boost/fusion/iterator/mpl/convert_iterator.hpp"
#include "third_party/boost/boost/fusion/adapted/mpl/mpl_iterator.hpp"
#include "third_party/boost/boost/fusion/iterator/value_of.hpp"
#include "third_party/boost/boost/fusion/support/iterator_base.hpp"
#include "third_party/boost/boost/fusion/algorithm/query/detail/find_if.hpp"
#include "third_party/boost/boost/mpl/lambda.hpp"
#include "third_party/boost/boost/mpl/quote.hpp"
#include "third_party/boost/boost/mpl/bind.hpp"
#include "third_party/boost/boost/mpl/placeholders.hpp"

#include "third_party/boost/boost/fusion/view/filter_view/detail/deref_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/next_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/value_of_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/equal_to_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/deref_data_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/value_of_data_impl.hpp"
#include "third_party/boost/boost/fusion/view/filter_view/detail/key_of_impl.hpp"

namespace boost { namespace fusion
{
    struct filter_view_iterator_tag;
    struct forward_traversal_tag;

    template <typename Category, typename First, typename Last, typename Pred>
    struct filter_iterator : iterator_base<filter_iterator<Category, First, Last, Pred> >
    {
        typedef convert_iterator<First> first_converter;
        typedef typename first_converter::type first_iter;
        typedef convert_iterator<Last> last_converter;
        typedef typename last_converter::type last_iter;

        typedef filter_view_iterator_tag fusion_tag;
        typedef Category category;
        typedef
            detail::static_find_if<
                first_iter
              , last_iter
              , mpl::bind1<
                    typename mpl::lambda<Pred>::type
                  , mpl::bind1<mpl::quote1<result_of::value_of>,mpl::_1>
                >
            >
        filter;
        typedef typename filter::type first_type;
        typedef last_iter last_type;
        typedef Pred pred_type;

        filter_iterator(First const& in_first)
            : first(filter::iter_call(first_converter::call(in_first))) {}

        first_type first;

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        filter_iterator& operator= (filter_iterator const&);
    };
}}

#endif


