//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_CONTAINERS_FLAT_SET_HPP
#define BOOST_INTERPROCESS_CONTAINERS_FLAT_SET_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/detail/config_begin.hpp"
#include "third_party/boost/boost/container/flat_set.hpp"
#include "third_party/boost/boost/interprocess/containers/containers_fwd.hpp"

namespace boost {
namespace interprocess {

using boost::container::flat_set;
using boost::container::flat_multiset;

}  //namespace interprocess {
}  //namespace boost {

#include "third_party/boost/boost/interprocess/detail/config_end.hpp"

#endif //   #ifndef  BOOST_INTERPROCESS_CONTAINERS_FLAT_SET_HPP
