//
// ssl/detail/shutdown_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP
#define BOOST_ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)
# include "third_party/boost/boost/asio/ssl/detail/engine.hpp"
#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace ssl {
namespace detail {

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)

class shutdown_op
{
public:
  engine::want operator()(engine& eng,
      boost::system::error_code& ec,
      std::size_t& bytes_transferred) const
  {
    bytes_transferred = 0;
    return eng.shutdown(ec);
  }

  template <typename Handler>
  void call_handler(Handler& handler,
      const boost::system::error_code& ec,
      const std::size_t&) const
  {
    handler(ec);
  }
};

#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // BOOST_ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP
