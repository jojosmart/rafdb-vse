//  chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008
//  Copyright Vicente J. Botet Escriba 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_CHRONO_DETAIL_INLINED_CHRONO_HPP
#define BOOST_CHRONO_DETAIL_INLINED_CHRONO_HPP

#include "third_party/boost/boost/version.hpp"
#include "third_party/boost/boost/chrono/chrono.hpp"
#include "third_party/boost/boost/system/system_error.hpp"
#include "third_party/boost/boost/throw_exception.hpp"
#include "third_party/boost/boost/chrono/detail/system.hpp"

//----------------------------------------------------------------------------//
//                                                                            //
//                     Platform-specific Implementations                      //
//                                                                            //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
//                                Windows                                     //
//----------------------------------------------------------------------------//
#if defined(BOOST_CHRONO_WINDOWS_API)
#include "third_party/boost/boost/chrono/detail/inlined/win/chrono.hpp"

//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_MAC_API)
#include "third_party/boost/boost/chrono/detail/inlined/mac/chrono.hpp"

//----------------------------------------------------------------------------//
//                                POSIX                                     //
//----------------------------------------------------------------------------//
#elif defined(BOOST_CHRONO_POSIX_API)
#include "third_party/boost/boost/chrono/detail/inlined/posix/chrono.hpp"

#endif  // POSIX

#endif
