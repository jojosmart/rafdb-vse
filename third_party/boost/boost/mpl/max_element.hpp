
#ifndef BOOST_MPL_MAX_ELEMENT_HPP_INCLUDED
#define BOOST_MPL_MAX_ELEMENT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: max_element.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 02:19:02 -0400 (Sat, 11 Oct 2008) $
// $Revision: 49267 $

#include "third_party/boost/boost/mpl/less.hpp"
#include "third_party/boost/boost/mpl/iter_fold.hpp"
#include "third_party/boost/boost/mpl/begin_end.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/deref.hpp"
#include "third_party/boost/boost/mpl/apply.hpp"
#include "third_party/boost/boost/mpl/aux_/common_name_wknd.hpp"
#include "third_party/boost/boost/mpl/aux_/na_spec.hpp"

namespace boost { namespace mpl {

BOOST_MPL_AUX_COMMON_NAME_WKND(max_element)

namespace aux {

template< typename Predicate >
struct select_max
{
    template< typename OldIterator, typename Iterator >
    struct apply
    {
        typedef typename apply2<
              Predicate
            , typename deref<OldIterator>::type
            , typename deref<Iterator>::type
            >::type condition_;

        typedef typename if_<
              condition_
            , Iterator
            , OldIterator
            >::type type;
    };
};

} // namespace aux 


template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename Predicate = less<_,_>
    >
struct max_element
    : iter_fold<
          Sequence
        , typename begin<Sequence>::type
        , protect< aux::select_max<Predicate> >
        >
{
};

BOOST_MPL_AUX_NA_SPEC(1, max_element)

}}

#endif // BOOST_MPL_MAX_ELEMENT_HPP_INCLUDED
