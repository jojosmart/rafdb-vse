//
// ssl/detail/stream_core.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SSL_DETAIL_STREAM_CORE_HPP
#define BOOST_ASIO_SSL_DETAIL_STREAM_CORE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)
# include "third_party/boost/boost/asio/deadline_timer.hpp"
# include "third_party/boost/boost/asio/ssl/detail/engine.hpp"
# include "third_party/boost/boost/asio/buffer.hpp"
#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace ssl {
namespace detail {

#if !defined(BOOST_ASIO_ENABLE_OLD_SSL)

struct stream_core
{
  // According to the OpenSSL documentation, this is the buffer size that is is
  // sufficient to hold the largest possible TLS record.
  enum { max_tls_record_size = 17 * 1024 };

  stream_core(SSL_CTX* context, boost::asio::io_service& io_service)
    : engine_(context),
      pending_read_(io_service),
      pending_write_(io_service),
      output_buffer_space_(max_tls_record_size),
      output_buffer_(boost::asio::buffer(output_buffer_space_)),
      input_buffer_space_(max_tls_record_size),
      input_buffer_(boost::asio::buffer(input_buffer_space_))
  {
    pending_read_.expires_at(boost::posix_time::neg_infin);
    pending_write_.expires_at(boost::posix_time::neg_infin);
  }

  ~stream_core()
  {
  }

  // The SSL engine.
  engine engine_;

  // Timer used for storing queued read operations.
  boost::asio::deadline_timer pending_read_;

  // Timer used for storing queued write operations.
  boost::asio::deadline_timer pending_write_;

  // Buffer space used to prepare output intended for the transport.
  std::vector<unsigned char> output_buffer_space_; 

  // A buffer that may be used to prepare output intended for the transport.
  const boost::asio::mutable_buffers_1 output_buffer_; 

  // Buffer space used to read input intended for the engine.
  std::vector<unsigned char> input_buffer_space_; 

  // A buffer that may be used to read input intended for the engine.
  const boost::asio::mutable_buffers_1 input_buffer_;

  // The buffer pointing to the engine's unconsumed input.
  boost::asio::const_buffer input_;
};

#endif // !defined(BOOST_ASIO_ENABLE_OLD_SSL)

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // BOOST_ASIO_SSL_DETAIL_STREAM_CORE_HPP
