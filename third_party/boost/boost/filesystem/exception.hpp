//  boost/filesystem/exception.hpp  ----------------------------------------------------//

//  Copyright Beman Dawes 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------// 

#ifndef BOOST_FILESYSTEM_EXCEPTIONX_HPP
#define BOOST_FILESYSTEM_EXCEPTIONX_HPP

#include "third_party/boost/boost/config.hpp"  // for <boost/config/user.hpp>, in case
                             //  BOOST_FILESYSTEM_VERSION defined there

# if defined(BOOST_FILESYSTEM_VERSION) \
  && BOOST_FILESYSTEM_VERSION != 2  && BOOST_FILESYSTEM_VERSION != 3
#   error BOOST_FILESYSTEM_VERSION defined, but not as 2 or 3
# endif

# if !defined(BOOST_FILESYSTEM_VERSION)
#   define BOOST_FILESYSTEM_VERSION 3
# endif

#if BOOST_FILESYSTEM_VERSION == 2
#  include "third_party/boost/boost/filesystem/v2/exception.hpp"

# else
#  include "third_party/boost/boost/filesystem/v3/exception.hpp"

# endif

#endif  // BOOST_FILESYSTEM_EXCEPTIONX_HPP 
