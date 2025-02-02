//
// high_resolution_timer.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_HIGH_RESOLUTION_TIMER_HPP
#define BOOST_ASIO_HIGH_RESOLUTION_TIMER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_ASIO_HAS_STD_CHRONO) \
  || defined(BOOST_ASIO_HAS_BOOST_CHRONO) \
  || defined(GENERATING_DOCUMENTATION)

#if defined(BOOST_ASIO_HAS_STD_CHRONO)
# include <chrono>
#elif defined(BOOST_ASIO_HAS_BOOST_CHRONO)
# include "third_party/boost/boost/chrono/system_clocks.hpp"
#endif

#include "third_party/boost/boost/asio/basic_waitable_timer.hpp"

namespace boost {
namespace asio {

#if defined(GENERATING_DOCUMENTATION)
/// Typedef for a timer based on the high resolution clock.
/**
 * This typedef uses the C++11 @c &lt;chrono&gt; standard library facility, if
 * available. Otherwise, it may use the Boost.Chrono library. To explicitly
 * utilise Boost.Chrono, use the basic_waitable_timer template directly:
 * @code
 * typedef basic_waitable_timer<boost::chrono::high_resolution_clock> timer;
 * @endcode
 */
typedef basic_waitable_timer<
    chrono::high_resolution_clock>
  high_resolution_timer;
#elif defined(BOOST_ASIO_HAS_STD_CHRONO)
typedef basic_waitable_timer<
    std::chrono::high_resolution_clock>
  high_resolution_timer;
#elif defined(BOOST_ASIO_HAS_BOOST_CHRONO)
typedef basic_waitable_timer<
    boost::chrono::high_resolution_clock>
  high_resolution_timer;
#endif

} // namespace asio
} // namespace boost

#endif // defined(BOOST_ASIO_HAS_STD_CHRONO) 
       //   || defined(BOOST_ASIO_HAS_BOOST_CHRONO)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // BOOST_ASIO_HIGH_RESOLUTION_TIMER_HPP
