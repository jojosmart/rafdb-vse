
#ifndef BOOST_MPL_INSERT_RANGE_HPP_INCLUDED
#define BOOST_MPL_INSERT_RANGE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: insert_range.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 02:19:02 -0400 (Sat, 11 Oct 2008) $
// $Revision: 49267 $

#include "third_party/boost/boost/mpl/insert_range_fwd.hpp"
#include "third_party/boost/boost/mpl/sequence_tag.hpp"
#include "third_party/boost/boost/mpl/aux_/insert_range_impl.hpp"
#include "third_party/boost/boost/mpl/aux_/na_spec.hpp"
#include "third_party/boost/boost/mpl/aux_/lambda_support.hpp"

namespace boost { namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename BOOST_MPL_AUX_NA_PARAM(Pos)
    , typename BOOST_MPL_AUX_NA_PARAM(Range)
    >
struct insert_range
    : insert_range_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence,Pos,Range >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(3,insert_range,(Sequence,Pos,Range))
};

BOOST_MPL_AUX_NA_SPEC(3, insert_range)

}}

#endif // BOOST_MPL_INSERT_RANGE_HPP_INCLUDED
