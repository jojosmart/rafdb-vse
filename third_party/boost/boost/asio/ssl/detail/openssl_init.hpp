//
// ssl/detail/openssl_init.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SSL_DETAIL_OPENSSL_INIT_HPP
#define BOOST_ASIO_SSL_DETAIL_OPENSSL_INIT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"
#include <cstring>
#include "third_party/boost/boost/asio/detail/noncopyable.hpp"
#include "third_party/boost/boost/asio/detail/shared_ptr.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace ssl {
namespace detail {

class openssl_init_base
  : private noncopyable
{
protected:
  // Class that performs the actual initialisation.
  class do_init;

  // Helper function to manage a do_init singleton. The static instance of the
  // openssl_init object ensures that this function is always called before
  // main, and therefore before any other threads can get started. The do_init
  // instance must be static in this function to ensure that it gets
  // initialised before any other global objects try to use it.
  BOOST_ASIO_DECL static boost::asio::detail::shared_ptr<do_init> instance();
};

template <bool Do_Init = true>
class openssl_init : private openssl_init_base
{
public:
  // Constructor.
  openssl_init()
    : ref_(instance())
  {
    using namespace std; // For memmove.

    // Ensure openssl_init::instance_ is linked in.
    openssl_init* tmp = &instance_;
    memmove(&tmp, &tmp, sizeof(openssl_init*));
  }

  // Destructor.
  ~openssl_init()
  {
  }

private:
  // Instance to force initialisation of openssl at global scope.
  static openssl_init instance_;

  // Reference to singleton do_init object to ensure that openssl does not get
  // cleaned up until the last user has finished with it.
  boost::asio::detail::shared_ptr<do_init> ref_;
};

template <bool Do_Init>
openssl_init<Do_Init> openssl_init<Do_Init>::instance_;

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#if defined(BOOST_ASIO_HEADER_ONLY)
# include "third_party/boost/boost/asio/ssl/detail/impl/openssl_init.ipp"
#endif // defined(BOOST_ASIO_HEADER_ONLY)

#endif // BOOST_ASIO_SSL_DETAIL_OPENSSL_INIT_HPP
