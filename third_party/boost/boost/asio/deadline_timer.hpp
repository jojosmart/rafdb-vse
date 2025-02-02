//
// deadline_timer.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DEADLINE_TIMER_HPP
#define BOOST_ASIO_DEADLINE_TIMER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"
#include "third_party/boost/boost/asio/detail/socket_types.hpp" // Must come before posix_time.
#include "third_party/boost/boost/asio/basic_deadline_timer.hpp"

#include "third_party/boost/boost/asio/detail/push_options.hpp"
#include "third_party/boost/boost/date_time/posix_time/posix_time_types.hpp"
#include "third_party/boost/boost/asio/detail/pop_options.hpp"

namespace boost {
namespace asio {

/// Typedef for the typical usage of timer. Uses a UTC clock.
typedef basic_deadline_timer<boost::posix_time::ptime> deadline_timer;

} // namespace asio
} // namespace boost

#endif // BOOST_ASIO_DEADLINE_TIMER_HPP
