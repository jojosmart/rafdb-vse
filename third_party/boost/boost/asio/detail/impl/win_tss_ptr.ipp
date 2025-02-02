//
// detail/impl/win_tss_ptr.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_IMPL_WIN_TSS_PTR_IPP
#define BOOST_ASIO_DETAIL_IMPL_WIN_TSS_PTR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_WINDOWS)

#include "third_party/boost/boost/asio/detail/throw_error.hpp"
#include "third_party/boost/boost/asio/detail/win_tss_ptr.hpp"
#include "third_party/boost/boost/asio/error.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace detail {

DWORD win_tss_ptr_create()
{
#if defined(UNDER_CE)
  enum { out_of_indexes = 0xFFFFFFFF };
#else
  enum { out_of_indexes = TLS_OUT_OF_INDEXES };
#endif

  DWORD tss_key = ::TlsAlloc();
  if (tss_key == out_of_indexes)
  {
    DWORD last_error = ::GetLastError();
    boost::system::error_code ec(last_error,
        boost::asio::error::get_system_category());
    boost::asio::detail::throw_error(ec, "tss");
  }
  return tss_key;
}

} // namespace detail
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // defined(BOOST_WINDOWS)

#endif // BOOST_ASIO_DETAIL_IMPL_WIN_TSS_PTR_IPP
