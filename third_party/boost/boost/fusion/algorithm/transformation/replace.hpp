/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_REPLACE_08182005_0830)
#define FUSION_REPLACE_08182005_0830

#include "third_party/boost/boost/fusion/view/transform_view/transform_view.hpp"
#include "third_party/boost/boost/fusion/algorithm/transformation/detail/replace.hpp"

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename T>
        struct replace
        {
            typedef transform_view<Sequence, detail::replacer<T> > type;
        };
    }

    template <typename Sequence, typename T>
    inline typename result_of::replace<Sequence const, T>::type
    replace(Sequence const& seq, T const& old_value, T const& new_value)
    {
        typedef typename result_of::replace<Sequence const, T>::type result;
        detail::replacer<T> f(old_value, new_value);
        return result(seq, f);
    }
}}

#endif

