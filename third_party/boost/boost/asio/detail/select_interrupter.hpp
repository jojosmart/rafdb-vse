//
// detail/select_interrupter.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP
#define BOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
# include "third_party/boost/boost/asio/detail/socket_select_interrupter.hpp"
#elif defined(BOOST_ASIO_HAS_EVENTFD)
# include "third_party/boost/boost/asio/detail/eventfd_select_interrupter.hpp"
#else
# include "third_party/boost/boost/asio/detail/pipe_select_interrupter.hpp"
#endif

namespace boost {
namespace asio {
namespace detail {

#if defined(BOOST_WINDOWS) || defined(__CYGWIN__) || defined(__SYMBIAN32__)
typedef socket_select_interrupter select_interrupter;
#elif defined(BOOST_ASIO_HAS_EVENTFD)
typedef eventfd_select_interrupter select_interrupter;
#else
typedef pipe_select_interrupter select_interrupter;
#endif

} // namespace detail
} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DETAIL_SELECT_INTERRUPTER_HPP
