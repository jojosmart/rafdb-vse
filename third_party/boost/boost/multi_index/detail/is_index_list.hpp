/* Copyright 2003-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_IS_INDEX_LIST_HPP
#define BOOST_MULTI_INDEX_DETAIL_IS_INDEX_LIST_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "third_party/boost/boost/config.hpp" /* keep it first to prevent nasty warns in MSVC */
#include "third_party/boost/boost/mpl/empty.hpp"
#include "third_party/boost/boost/mpl/is_sequence.hpp"

namespace boost{

namespace multi_index{

namespace detail{

template<typename T>
struct is_index_list
{
  BOOST_STATIC_CONSTANT(bool,mpl_sequence=mpl::is_sequence<T>::value);
  BOOST_STATIC_CONSTANT(bool,non_empty=!mpl::empty<T>::value);
  BOOST_STATIC_CONSTANT(bool,value=mpl_sequence&&non_empty);
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace boost */

#endif
