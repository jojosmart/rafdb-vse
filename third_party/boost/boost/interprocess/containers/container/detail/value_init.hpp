//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2009.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONTAINERS_DETAIL_VALUE_INIT_HPP
#define BOOST_CONTAINERS_DETAIL_VALUE_INIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/containers/container/detail/config_begin.hpp"
#include "third_party/boost/boost/interprocess/containers/container/detail/workaround.hpp"

namespace boost {
namespace interprocess_container { 
namespace containers_detail {

template<class T>
struct value_init
{
   value_init()
      : m_t()
   {}

   T m_t;
};

}  //namespace containers_detail { 
}  //namespace interprocess_container { 
}  //namespace boost {

#include "third_party/boost/boost/interprocess/containers/container/detail/config_end.hpp"

#endif   //#ifndef BOOST_CONTAINERS_DETAIL_VALUE_INIT_HPP
