/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_DEREF_DATA_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_DEREF_DATA_IMPL_HPP

#include "third_party/boost/boost/fusion/iterator/value_of.hpp"
#include "third_party/boost/boost/fusion/iterator/deref.hpp"
#include "third_party/boost/boost/fusion/support/detail/access.hpp"
#include "third_party/boost/boost/type_traits/is_const.hpp"
#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_data_impl;

    template <>
    struct deref_data_impl<map_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename result_of::value_of<It>::type::second_type data;

            typedef typename
                mpl::eval_if<
                    is_const<typename It::seq_type>
                  , detail::cref_result<mpl::identity<data> >
                  , detail::ref_result<mpl::identity<data> >
                >::type
            type;

            static type
            call(It const& it)
            {
                return fusion::deref(it).second;
            }
        };
    };
}}}

#endif
