/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_END_IMPL_07172005_0828)
#define FUSION_END_IMPL_07172005_0828

#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/type_traits/is_const.hpp"

namespace boost { namespace fusion
{
    struct nil;

    struct cons_tag;

    template <typename Car, typename Cdr>
    struct cons;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<cons_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef cons_iterator<
                    typename mpl::if_<is_const<Sequence>, nil const, nil>::type>
                type;
    
                static type
                call(Sequence&)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
