/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_SIZE_IMPL_24122005_1759)
#define BOOST_FUSION_SIZE_IMPL_24122005_1759

#include "third_party/boost/boost/mpl/int.hpp"

namespace boost { namespace fusion {

    struct std_pair_tag;

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template <>
        struct size_impl<std_pair_tag>
        {
            template <typename Sequence>
            struct apply : mpl::int_<2> {};
        };
    }
}}

#endif
