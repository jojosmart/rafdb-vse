//
// detail/atomic_count.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_ATOMIC_COUNT_HPP
#define BOOST_ASIO_DETAIL_ATOMIC_COUNT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
// Nothing to include.
#elif defined(BOOST_ASIO_HAS_STD_ATOMIC)
# include <atomic>
#else // defined(BOOST_ASIO_HAS_STD_ATOMIC)
# include "third_party/boost/boost/detail/atomic_count.hpp"
#endif // defined(BOOST_ASIO_HAS_STD_ATOMIC)

namespace boost {
namespace asio {
namespace detail {

#if !defined(BOOST_HAS_THREADS) || defined(BOOST_ASIO_DISABLE_THREADS)
typedef long atomic_count;
#elif defined(BOOST_ASIO_HAS_STD_ATOMIC)
typedef std::atomic<long> atomic_count;
#else // defined(BOOST_ASIO_HAS_STD_ATOMIC)
typedef boost::detail::atomic_count atomic_count;
#endif // defined(BOOST_ASIO_HAS_STD_ATOMIC)

} // namespace detail
} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DETAIL_ATOMIC_COUNT_HPP
