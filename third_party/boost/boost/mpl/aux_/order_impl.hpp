
#ifndef BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: order_impl.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-11 02:19:02 -0400 (Sat, 11 Oct 2008) $
// $Revision: 49267 $

#include "third_party/boost/boost/mpl/order_fwd.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/long.hpp"
#include "third_party/boost/boost/mpl/has_key.hpp"
#include "third_party/boost/boost/mpl/aux_/overload_names.hpp"
#include "third_party/boost/boost/mpl/aux_/static_cast.hpp"
#include "third_party/boost/boost/mpl/aux_/type_wrapper.hpp"
#include "third_party/boost/boost/mpl/aux_/traits_lambda_spec.hpp"
#include "third_party/boost/boost/mpl/aux_/config/msvc.hpp"
#include "third_party/boost/boost/mpl/aux_/config/static_constant.hpp"
#include "third_party/boost/boost/mpl/aux_/config/workaround.hpp"

namespace boost { namespace mpl {

// default implementation; requires 'Seq' to provide corresponding overloads 
// of BOOST_MPL_AUX_OVERLOAD_ORDER_BY_KEY

template< typename Seq, typename Key > struct x_order_impl
#if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400)) \
    || BOOST_WORKAROUND(__EDG_VERSION__, <= 245)
{
    BOOST_STATIC_CONSTANT(long, value = 
          sizeof( BOOST_MPL_AUX_OVERLOAD_CALL_ORDER_BY_KEY(
              Seq
            , BOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<Key>*, 0)
            ) )
        );

    typedef long_<value> type;

#else // ISO98 C++
    : long_< 
          sizeof( BOOST_MPL_AUX_OVERLOAD_CALL_ORDER_BY_KEY(
              Seq
            , BOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<Key>*, 0)
            ) )
        >
{
#endif
};

template< typename Tag >
struct order_impl
{
    template< typename Seq, typename Key > struct apply
        : if_<
              typename has_key_impl<Tag>::template apply<Seq,Key>
            , x_order_impl<Seq,Key>
            , void_
            >::type
    {
    };
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2,order_impl)

}}

#endif // BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED
