/* Copyright 2003-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_NODE_TYPE_HPP
#define BOOST_MULTI_INDEX_DETAIL_NODE_TYPE_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "third_party/boost/boost/config.hpp" /* keep it first to prevent nasty warns in MSVC */
#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/mpl/bind.hpp"
#include "third_party/boost/boost/mpl/reverse_iter_fold.hpp"
#include "third_party/boost/boost/mpl/deref.hpp"
#include "third_party/boost/boost/multi_index_container_fwd.hpp"
#include "third_party/boost/boost/multi_index/detail/header_holder.hpp"
#include "third_party/boost/boost/multi_index/detail/index_node_base.hpp"
#include "third_party/boost/boost/multi_index/detail/is_index_list.hpp"
#include "third_party/boost/boost/multi_index/detail/msvc_index_specifier.hpp"
#include "third_party/boost/boost/static_assert.hpp"

namespace boost{

namespace multi_index{

namespace detail{

/* MPL machinery to construct the internal node type associated to an
 * index list.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1310)
struct index_node_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply:
    msvc_index_specifier< mpl::deref<IndexSpecifierIterator>::type >::
      template result_node_class<Super>
  {
  }; 
};
#else
struct index_node_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply
  {
    typedef typename mpl::deref<IndexSpecifierIterator>::type index_specifier;
    typedef typename index_specifier::
      BOOST_NESTED_TEMPLATE node_class<Super>::type type;
  }; 
};
#endif

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct multi_index_node_type
{
  BOOST_STATIC_ASSERT(detail::is_index_list<IndexSpecifierList>::value);

  typedef typename mpl::reverse_iter_fold<
    IndexSpecifierList,
    index_node_base<Value,Allocator>,
    mpl::bind2<index_node_applier,mpl::_2,mpl::_1>
  >::type type;
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
