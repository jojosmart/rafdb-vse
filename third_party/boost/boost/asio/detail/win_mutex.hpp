//
// detail/win_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_WIN_MUTEX_HPP
#define BOOST_ASIO_DETAIL_WIN_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_WINDOWS)

#include "third_party/boost/boost/asio/detail/noncopyable.hpp"
#include "third_party/boost/boost/asio/detail/scoped_lock.hpp"
#include "third_party/boost/boost/asio/detail/socket_types.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace detail {

class win_mutex
  : private noncopyable
{
public:
  typedef boost::asio::detail::scoped_lock<win_mutex> scoped_lock;

  // Constructor.
  BOOST_ASIO_DECL win_mutex();

  // Destructor.
  ~win_mutex()
  {
    ::DeleteCriticalSection(&crit_section_);
  }

  // Lock the mutex.
  void lock()
  {
    ::EnterCriticalSection(&crit_section_);
  }

  // Unlock the mutex.
  void unlock()
  {
    ::LeaveCriticalSection(&crit_section_);
  }

private:
  // Initialisation must be performed in a separate function to the constructor
  // since the compiler does not support the use of structured exceptions and
  // C++ exceptions in the same function.
  BOOST_ASIO_DECL int do_init();

  ::CRITICAL_SECTION crit_section_;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#if defined(BOOST_ASIO_HEADER_ONLY)
# include "third_party/boost/boost/asio/detail/impl/win_mutex.ipp"
#endif // defined(BOOST_ASIO_HEADER_ONLY)

#endif // defined(BOOST_WINDOWS)

#endif // BOOST_ASIO_DETAIL_WIN_MUTEX_HPP
