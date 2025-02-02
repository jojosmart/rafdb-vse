//
// impl/io_service.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_IMPL_IO_SERVICE_HPP
#define BOOST_ASIO_IMPL_IO_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/handler_type_requirements.hpp"
#include "third_party/boost/boost/asio/detail/service_registry.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {

template <typename Service>
inline Service& use_service(io_service& ios)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<io_service::service*>(static_cast<Service*>(0));
  (void)static_cast<const io_service::id*>(&Service::id);

  return ios.service_registry_->template use_service<Service>();
}

template <>
inline detail::io_service_impl& use_service<detail::io_service_impl>(
    io_service& ios)
{
  return ios.impl_;
}

template <typename Service>
inline void add_service(io_service& ios, Service* svc)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<io_service::service*>(static_cast<Service*>(0));
  (void)static_cast<const io_service::id*>(&Service::id);

  ios.service_registry_->template add_service<Service>(svc);
}

template <typename Service>
inline bool has_service(io_service& ios)
{
  // Check that Service meets the necessary type requirements.
  (void)static_cast<io_service::service*>(static_cast<Service*>(0));
  (void)static_cast<const io_service::id*>(&Service::id);

  return ios.service_registry_->template has_service<Service>();
}

} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#if defined(BOOST_ASIO_HAS_IOCP)
# include "third_party/boost/boost/asio/detail/win_iocp_io_service.hpp"
#else
# include "third_party/boost/boost/asio/detail/task_io_service.hpp"
#endif

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {

template <typename CompletionHandler>
inline void io_service::dispatch(
    BOOST_ASIO_MOVE_ARG(CompletionHandler) handler)
{
  // If you get an error on the following line it means that your handler does
  // not meet the documented type requirements for a CompletionHandler.
  BOOST_ASIO_COMPLETION_HANDLER_CHECK(CompletionHandler, handler) type_check;

  impl_.dispatch(BOOST_ASIO_MOVE_CAST(CompletionHandler)(handler));
}

template <typename CompletionHandler>
inline void io_service::post(BOOST_ASIO_MOVE_ARG(CompletionHandler) handler)
{
  // If you get an error on the following line it means that your handler does
  // not meet the documented type requirements for a CompletionHandler.
  BOOST_ASIO_COMPLETION_HANDLER_CHECK(CompletionHandler, handler) type_check;

  impl_.post(BOOST_ASIO_MOVE_CAST(CompletionHandler)(handler));
}

template <typename Handler>
#if defined(GENERATING_DOCUMENTATION)
unspecified
#else
inline detail::wrapped_handler<io_service&, Handler>
#endif
io_service::wrap(Handler handler)
{
  return detail::wrapped_handler<io_service&, Handler>(*this, handler);
}

inline io_service::work::work(boost::asio::io_service& io_service)
  : io_service_impl_(io_service.impl_)
{
  io_service_impl_.work_started();
}

inline io_service::work::work(const work& other)
  : io_service_impl_(other.io_service_impl_)
{
  io_service_impl_.work_started();
}

inline io_service::work::~work()
{
  io_service_impl_.work_finished();
}

inline boost::asio::io_service& io_service::work::get_io_service()
{
  return io_service_impl_.get_io_service();
}

inline boost::asio::io_service& io_service::service::get_io_service()
{
  return owner_;
}

} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // BOOST_ASIO_IMPL_IO_SERVICE_HPP
