/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_END_IMPL_24122005_1755)
#define BOOST_FUSION_END_IMPL_24122005_1755

#include "third_party/boost/boost/fusion/adapted/std_pair/std_pair_iterator.hpp"

namespace boost { namespace fusion {

    struct std_pair_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<std_pair_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef std_pair_iterator<Sequence, 2> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
