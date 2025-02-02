//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONTAINER_DETAIL_VALUE_INIT_HPP
#define BOOST_CONTAINER_DETAIL_VALUE_INIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "config_begin.hpp"
#include "third_party/boost/boost/container/detail/workaround.hpp"

namespace boost {
namespace container { 
namespace container_detail {

template<class T>
struct value_init
{
   value_init()
      : m_t()
   {}

   operator T &() { return m_t; }

   T m_t;
};

}  //namespace container_detail { 
}  //namespace container { 
}  //namespace boost {

#include "third_party/boost/boost/container/detail/config_end.hpp"

#endif   //#ifndef BOOST_CONTAINER_DETAIL_VALUE_INIT_HPP
