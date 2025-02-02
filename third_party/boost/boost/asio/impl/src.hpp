//
// impl/src.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_IMPL_SRC_HPP
#define BOOST_ASIO_IMPL_SRC_HPP

#define BOOST_ASIO_SOURCE

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_ASIO_HEADER_ONLY)
# error Do not compile Asio library source with BOOST_ASIO_HEADER_ONLY defined
#endif

#include "third_party/boost/boost/asio/impl/error.ipp"
#include "third_party/boost/boost/asio/impl/io_service.ipp"
#include "third_party/boost/boost/asio/impl/serial_port_base.ipp"
#include "third_party/boost/boost/asio/detail/impl/descriptor_ops.ipp"
#include "third_party/boost/boost/asio/detail/impl/dev_poll_reactor.ipp"
#include "third_party/boost/boost/asio/detail/impl/epoll_reactor.ipp"
#include "third_party/boost/boost/asio/detail/impl/eventfd_select_interrupter.ipp"
#include "third_party/boost/boost/asio/detail/impl/handler_tracking.ipp"
#include "third_party/boost/boost/asio/detail/impl/kqueue_reactor.ipp"
#include "third_party/boost/boost/asio/detail/impl/pipe_select_interrupter.ipp"
#include "third_party/boost/boost/asio/detail/impl/posix_event.ipp"
#include "third_party/boost/boost/asio/detail/impl/posix_mutex.ipp"
#include "third_party/boost/boost/asio/detail/impl/posix_thread.ipp"
#include "third_party/boost/boost/asio/detail/impl/posix_tss_ptr.ipp"
#include "third_party/boost/boost/asio/detail/impl/reactive_descriptor_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/reactive_serial_port_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/reactive_socket_service_base.ipp"
#include "third_party/boost/boost/asio/detail/impl/resolver_service_base.ipp"
#include "third_party/boost/boost/asio/detail/impl/select_reactor.ipp"
#include "third_party/boost/boost/asio/detail/impl/service_registry.ipp"
#include "third_party/boost/boost/asio/detail/impl/signal_set_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/socket_ops.ipp"
#include "third_party/boost/boost/asio/detail/impl/socket_select_interrupter.ipp"
#include "third_party/boost/boost/asio/detail/impl/strand_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/task_io_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/throw_error.ipp"
#include "third_party/boost/boost/asio/detail/impl/timer_queue_ptime.ipp"
#include "third_party/boost/boost/asio/detail/impl/timer_queue_set.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_iocp_handle_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_iocp_io_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_iocp_serial_port_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_iocp_socket_service_base.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_event.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_mutex.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_object_handle_service.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_static_mutex.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_thread.ipp"
#include "third_party/boost/boost/asio/detail/impl/win_tss_ptr.ipp"
#include "third_party/boost/boost/asio/detail/impl/winsock_init.ipp"
#include "third_party/boost/boost/asio/ip/impl/address.ipp"
#include "third_party/boost/boost/asio/ip/impl/address_v4.ipp"
#include "third_party/boost/boost/asio/ip/impl/address_v6.ipp"
#include "third_party/boost/boost/asio/ip/impl/host_name.ipp"
#include "third_party/boost/boost/asio/ip/detail/impl/endpoint.ipp"
#include "third_party/boost/boost/asio/local/detail/impl/endpoint.ipp"

#endif // BOOST_ASIO_IMPL_SRC_HPP
