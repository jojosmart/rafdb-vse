//
// serial_port_base.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2008 Rep Invariant Systems, Inc. (info@repinvariant.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SERIAL_PORT_BASE_HPP
#define BOOST_ASIO_SERIAL_PORT_BASE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "third_party/boost/boost/asio/detail/config.hpp"

#if defined(BOOST_ASIO_HAS_SERIAL_PORT) \
  || defined(GENERATING_DOCUMENTATION)

#if !defined(BOOST_WINDOWS) && !defined(__CYGWIN__)
# include <termios.h>
#endif // !defined(BOOST_WINDOWS) && !defined(__CYGWIN__)

#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/asio/detail/socket_types.hpp"
#include "third_party/boost/boost/system/error_code.hpp"

#if defined(GENERATING_DOCUMENTATION)
# define BOOST_ASIO_OPTION_STORAGE implementation_defined
#elif defined(BOOST_WINDOWS) || defined(__CYGWIN__)
# define BOOST_ASIO_OPTION_STORAGE DCB
#else
# define BOOST_ASIO_OPTION_STORAGE termios
#endif

#include "third_party/boost/boost/asio/detail/push_options.hpp"

namespace boost {
namespace asio {

/// The serial_port_base class is used as a base for the basic_serial_port class
/// template so that we have a common place to define the serial port options.
class serial_port_base
{
public:
  /// Serial port option to permit changing the baud rate.
  /**
   * Implements changing the baud rate for a given serial port.
   */
  class baud_rate
  {
  public:
    explicit baud_rate(unsigned int rate = 0);
    unsigned int value() const;
    BOOST_ASIO_DECL boost::system::error_code store(
        BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec) const;
    BOOST_ASIO_DECL boost::system::error_code load(
        const BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec);
  private:
    unsigned int value_;
  };

  /// Serial port option to permit changing the flow control.
  /**
   * Implements changing the flow control for a given serial port.
   */
  class flow_control
  {
  public:
    enum type { none, software, hardware };
    BOOST_ASIO_DECL explicit flow_control(type t = none);
    type value() const;
    BOOST_ASIO_DECL boost::system::error_code store(
        BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec) const;
    BOOST_ASIO_DECL boost::system::error_code load(
        const BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec);
  private:
    type value_;
  };

  /// Serial port option to permit changing the parity.
  /**
   * Implements changing the parity for a given serial port.
   */
  class parity
  {
  public:
    enum type { none, odd, even };
    BOOST_ASIO_DECL explicit parity(type t = none);
    type value() const;
    BOOST_ASIO_DECL boost::system::error_code store(
        BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec) const;
    BOOST_ASIO_DECL boost::system::error_code load(
        const BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec);
  private:
    type value_;
  };

  /// Serial port option to permit changing the number of stop bits.
  /**
   * Implements changing the number of stop bits for a given serial port.
   */
  class stop_bits
  {
  public:
    enum type { one, onepointfive, two };
    BOOST_ASIO_DECL explicit stop_bits(type t = one);
    type value() const;
    BOOST_ASIO_DECL boost::system::error_code store(
        BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec) const;
    BOOST_ASIO_DECL boost::system::error_code load(
        const BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec);
  private:
    type value_;
  };

  /// Serial port option to permit changing the character size.
  /**
   * Implements changing the character size for a given serial port.
   */
  class character_size
  {
  public:
    BOOST_ASIO_DECL explicit character_size(unsigned int t = 8);
    unsigned int value() const;
    BOOST_ASIO_DECL boost::system::error_code store(
        BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec) const;
    BOOST_ASIO_DECL boost::system::error_code load(
        const BOOST_ASIO_OPTION_STORAGE& storage,
        boost::system::error_code& ec);
  private:
    unsigned int value_;
  };

protected:
  /// Protected destructor to prevent deletion through this type.
  ~serial_port_base()
  {
  }

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
private:
  // Workaround to enable the empty base optimisation with Borland C++.
  char dummy_;
#endif
};

} // namespace asio
} // namespace boost

#include "third_party/boost/boost/asio/detail/pop_options.hpp"

#undef BOOST_ASIO_OPTION_STORAGE

#include "third_party/boost/boost/asio/impl/serial_port_base.hpp"
#if defined(BOOST_ASIO_HEADER_ONLY)
# include "third_party/boost/boost/asio/impl/serial_port_base.ipp"
#endif // defined(BOOST_ASIO_HEADER_ONLY)

#endif // defined(BOOST_ASIO_HAS_SERIAL_PORT)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // BOOST_ASIO_SERIAL_PORT_BASE_HPP
