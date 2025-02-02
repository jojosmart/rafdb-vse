/* Copyright 2003-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_BASE_TYPE_HPP
#define BOOST_MULTI_INDEX_DETAIL_BASE_TYPE_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "third_party/boost/boost/config.hpp" /* keep it first to prevent nasty warns in MSVC */
#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/mpl/at.hpp"
#include "third_party/boost/boost/mpl/apply.hpp"
#include "third_party/boost/boost/mpl/size.hpp"
#include "third_party/boost/boost/multi_index/detail/index_base.hpp"
#include "third_party/boost/boost/multi_index/detail/is_index_list.hpp"
#include "third_party/boost/boost/multi_index/detail/msvc_index_specifier.hpp"
#include "third_party/boost/boost/static_assert.hpp"

namespace boost{

namespace multi_index{

namespace detail{

/* MPL machinery to construct a linear hierarchy of indices out of
 * a index list.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1310)
struct index_applier
{
  template<typename IndexSpecifierMeta,typename SuperMeta>
  struct apply:
    msvc_index_specifier<IndexSpecifierMeta::type>::
      template result_index_class<SuperMeta>
  {
  }; 
};
#else
struct index_applier
{
  template<typename IndexSpecifierMeta,typename SuperMeta>
  struct apply
  {
    typedef typename IndexSpecifierMeta::type            index_specifier;
    typedef typename index_specifier::
      BOOST_NESTED_TEMPLATE index_class<SuperMeta>::type type;
  }; 
};
#endif

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer
{
  BOOST_STATIC_CONSTANT(int,length=mpl::size<IndexSpecifierList>::value);

  typedef typename  mpl::eval_if_c<
    N==length,
    mpl::identity<index_base<Value,IndexSpecifierList,Allocator> >,
    mpl::apply2<
      index_applier,
      mpl::at_c<IndexSpecifierList,N>,
      nth_layer<N+1,Value,IndexSpecifierList,Allocator>
    >
  >::type type;
};

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct multi_index_base_type:nth_layer<0,Value,IndexSpecifierList,Allocator>
{
  BOOST_STATIC_ASSERT(detail::is_index_list<IndexSpecifierList>::value);
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
