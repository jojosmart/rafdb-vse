
#ifndef BOOST_MPL_MAP_AUX_INSERT_IMPL_HPP_INCLUDED
#define BOOST_MPL_MAP_AUX_INSERT_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: insert_impl.hpp 55751 2009-08-24 04:11:00Z agurtovoy $
// $Date: 2009-08-24 00:11:00 -0400 (Mon, 24 Aug 2009) $
// $Revision: 55751 $

#include "third_party/boost/boost/mpl/insert_fwd.hpp"
#include "third_party/boost/boost/mpl/next_prior.hpp"
#include "third_party/boost/boost/mpl/map/aux_/contains_impl.hpp"
#include "third_party/boost/boost/mpl/map/aux_/item.hpp"
#include "third_party/boost/boost/mpl/map/aux_/tag.hpp"
#include "third_party/boost/boost/mpl/aux_/na.hpp"
#include "third_party/boost/boost/mpl/aux_/config/typeof.hpp"

namespace boost { namespace mpl {

namespace aux {
template< typename Map, typename Pair > 
struct map_insert_impl
    : if_< 
          contains_impl<aux::map_tag>::apply<Map,Pair>
        , Map
#if defined(BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)
        , m_item<
              typename Pair::first
            , typename Pair::second
            , Map
            >
#else
        , m_item<
              Map::order::value
            , typename Pair::first
            , typename Pair::second
            , Map
            >
#endif
        >
{
};
}

template<>
struct insert_impl< aux::map_tag >
{
    template< 
          typename Map
        , typename PosOrKey
        , typename KeyOrNA
        > 
    struct apply
        : aux::map_insert_impl<
              Map
            , typename if_na<KeyOrNA,PosOrKey>::type
            >
    {
    };
};

}}

#endif // BOOST_MPL_MAP_AUX_INSERT_IMPL_HPP_INCLUDED
