//
// detail/fenced_block.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_FENCED_BLOCK_HPP
#define BOOST_ASIO_DETAIL_FENCED_BLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if !defined(BOOST_HAS_THREADS) \
  || defined(BOOST_ASIO_DISABLE_THREADS) \
  || defined(BOOST_ASIO_DISABLE_FENCED_BLOCK)
# include "third_party/boost/boost/asio/detail/null_fenced_block.hpp"
#elif defined(__MACH__) && defined(__APPLE__)
# include "third_party/boost/boost/asio/detail/macos_fenced_block.hpp"
#elif defined(__sun)
# include "third_party/boost/boost/asio/detail/solaris_fenced_block.hpp"
#elif defined(__GNUC__) && defined(__arm__)
# include "third_party/boost/boost/asio/detail/gcc_arm_fenced_block.hpp"
#elif defined(__GNUC__) && (defined(__hppa) || defined(__hppa__))
# include "third_party/boost/boost/asio/detail/gcc_hppa_fenced_block.hpp"
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
# include "third_party/boost/boost/asio/detail/gcc_x86_fenced_block.hpp"
#elif defined(__GNUC__) \
  && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)) \
  && !defined(__INTEL_COMPILER) && !defined(__ICL) \
  && !defined(__ICC) && !defined(__ECC) && !defined(__PATHSCALE__)
# include "third_party/boost/boost/asio/detail/gcc_sync_fenced_block.hpp"
#elif defined(BOOST_WINDOWS) && !defined(UNDER_CE)
# include "third_party/boost/boost/asio/detail/win_fenced_block.hpp"
#else
# include "third_party/boost/boost/asio/detail/null_fenced_block.hpp"
#endif

namespace boost {
namespace asio {
namespace detail {

#if !defined(BOOST_HAS_THREADS) \
  || defined(BOOST_ASIO_DISABLE_THREADS) \
  || defined(BOOST_ASIO_DISABLE_FENCED_BLOCK)
typedef null_fenced_block fenced_block;
#elif defined(__MACH__) && defined(__APPLE__)
typedef macos_fenced_block fenced_block;
#elif defined(__sun)
typedef solaris_fenced_block fenced_block;
#elif defined(__GNUC__) && defined(__arm__)
typedef gcc_arm_fenced_block fenced_block;
#elif defined(__GNUC__) && (defined(__hppa) || defined(__hppa__))
typedef gcc_hppa_fenced_block fenced_block;
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
typedef gcc_x86_fenced_block fenced_block;
#elif defined(__GNUC__) \
  && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)) \
  && !defined(__INTEL_COMPILER) && !defined(__ICL) \
  && !defined(__ICC) && !defined(__ECC) && !defined(__PATHSCALE__)
typedef gcc_sync_fenced_block fenced_block;
#elif defined(BOOST_WINDOWS) && !defined(UNDER_CE)
typedef win_fenced_block fenced_block;
#else
typedef null_fenced_block fenced_block;
#endif

} // namespace detail
} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DETAIL_FENCED_BLOCK_HPP
