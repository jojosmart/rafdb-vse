/*=============================================================================
    Copyright (c) 2005-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_DEQUE_BEGIN_IMPL_09122006_2034)
#define BOOST_FUSION_DEQUE_BEGIN_IMPL_09122006_2034

#include "third_party/boost/boost/fusion/container/deque/deque_iterator.hpp"

#include "third_party/boost/boost/mpl/equal_to.hpp"
#include "third_party/boost/boost/mpl/if.hpp"

namespace boost { namespace fusion { 

    struct deque_tag;

    namespace extension 
    {
        template<typename T>
        struct begin_impl;
        
        template<>
        struct begin_impl<deque_tag>
        {
            template<typename Sequence>
            struct apply
            {
                typedef typename mpl::if_<
                    mpl::equal_to<typename Sequence::next_down, typename Sequence::next_up>,
                    deque_iterator<Sequence, 0>,
                    deque_iterator<
                    Sequence, mpl::plus<typename Sequence::next_down, mpl::int_<1> >::value> >::type type;
                
                static type call(Sequence& seq)
                {
                    return type(seq);
                }
            };
        };
    }
}}

#endif
