//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_DETAIL_WINDOWS_RECURSIVE_MUTEX_HPP
#define BOOST_INTERPROCESS_DETAIL_WINDOWS_RECURSIVE_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/detail/config_begin.hpp"
#include "third_party/boost/boost/interprocess/detail/workaround.hpp"
#include "third_party/boost/boost/interprocess/sync/windows/mutex.hpp"

namespace boost {
namespace interprocess {
namespace ipcdetail {

//Windows mutex is already recursive
class windows_recursive_mutex
   : public windows_mutex
{
   windows_recursive_mutex(const windows_recursive_mutex &);
   windows_recursive_mutex &operator=(const windows_recursive_mutex &);
   public:
   windows_recursive_mutex() : windows_mutex() {}
};

}  //namespace ipcdetail {
}  //namespace interprocess {
}  //namespace boost {


#include "third_party/boost/boost/interprocess/detail/config_end.hpp"

#endif   //BOOST_INTERPROCESS_DETAIL_WINDOWS_RECURSIVE_MUTEX_HPP
