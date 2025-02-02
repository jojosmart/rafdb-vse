/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_AT_IMPL_09242011_1744)
#define BOOST_FUSION_AT_IMPL_09242011_1744

#include <tuple>
#include <utility>
#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/fusion/support/detail/access.hpp"
#include "third_party/boost/boost/type_traits/remove_const.hpp"

namespace boost { namespace fusion
{
    struct std_tuple_tag;

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template <>
        struct at_impl<std_tuple_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                typedef typename remove_const<Sequence>::type seq_type;
                typedef std::tuple_element<N::value, seq_type> element;

                typedef typename
                    mpl::eval_if<
                        is_const<Sequence>
                      , fusion::detail::cref_result<element>
                      , fusion::detail::ref_result<element>
                    >::type
                type;

                static type
                call(Sequence& seq)
                {
                    return std::get<N::value>(seq);
                }
            };
        };
    }
}}

#endif
