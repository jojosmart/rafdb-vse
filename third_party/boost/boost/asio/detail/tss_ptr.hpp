//
// detail/tss_ptr.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_TSS_PTR_HPP
#define BOOST_ASIO_DETAIL_TSS_PTR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
# include "third_party/boost/boost/asio/detail/null_tss_ptr.hpp"
#elif defined(BOOST_WINDOWS)
# include "third_party/boost/boost/asio/detail/win_tss_ptr.hpp"
#elif defined(BOOST_HAS_PTHREADS)
# include "third_party/boost/boost/asio/detail/posix_tss_ptr.hpp"
#else
# error Only Windows and POSIX are supported!
#endif

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {
namespace detail {

template <typename T>
class tss_ptr
#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
  : public null_tss_ptr<T>
#elif defined(BOOST_WINDOWS)
  : public win_tss_ptr<T>
#elif defined(BOOST_HAS_PTHREADS)
  : public posix_tss_ptr<T>
#endif
{
public:
  void operator=(T* value)
  {
#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
    null_tss_ptr<T>::operator=(value);
#elif defined(BOOST_WINDOWS)
    win_tss_ptr<T>::operator=(value);
#elif defined(BOOST_HAS_PTHREADS)
    posix_tss_ptr<T>::operator=(value);
#endif
  }
};

} // namespace detail
} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#endif // BOOST_ASIO_DETAIL_TSS_PTR_HPP
