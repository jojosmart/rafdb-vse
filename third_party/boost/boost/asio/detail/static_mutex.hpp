//
// detail/static_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_STATIC_MUTEX_HPP
#define BOOST_ASIO_DETAIL_STATIC_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
# include "third_party/boost/boost/asio/detail/null_static_mutex.hpp"
#elif defined(BOOST_WINDOWS)
# include "third_party/boost/boost/asio/detail/win_static_mutex.hpp"
#elif defined(BOOST_HAS_PTHREADS)
# include "third_party/boost/boost/asio/detail/posix_static_mutex.hpp"
#else
# error Only Windows and POSIX are supported!
#endif

namespace boost {
namespace asio {
namespace detail {

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
typedef null_static_mutex static_mutex;
# define BOOST_ASIO_STATIC_MUTEX_INIT BOOST_ASIO_NULL_STATIC_MUTEX_INIT
#elif defined(BOOST_WINDOWS)
typedef win_static_mutex static_mutex;
# define BOOST_ASIO_STATIC_MUTEX_INIT BOOST_ASIO_WIN_STATIC_MUTEX_INIT
#elif defined(BOOST_HAS_PTHREADS)
typedef posix_static_mutex static_mutex;
# define BOOST_ASIO_STATIC_MUTEX_INIT BOOST_ASIO_POSIX_STATIC_MUTEX_INIT
#endif

} // namespace detail
} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DETAIL_STATIC_MUTEX_HPP
