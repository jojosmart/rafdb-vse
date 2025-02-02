//
// detail/null_thread.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_NULL_THREAD_HPP
#define BOOST_ASIO_DETAIL_NULL_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)

#include "third_party/boost/boost/asio/detail/noncopyable.hpp"
#include "third_party/boost/boost/asio/detail/throw_error.hpp"
#include "third_party/boost/boost/asio/error.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace detail {

class null_thread
  : private noncopyable
{
public:
  // Constructor.
  template <typename Function>
  null_thread(Function, unsigned int = 0)
  {
    boost::asio::detail::throw_error(
        boost::asio::error::operation_not_supported, "thread");
  }

  // Destructor.
  ~null_thread()
  {
  }

  // Wait for the thread to exit.
  void join()
  {
  }
};

} // namespace detail
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)

#endif // BOOST_ASIO_DETAIL_NULL_THREAD_HPP
