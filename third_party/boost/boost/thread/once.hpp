#ifndef BOOST_THREAD_ONCE_HPP
#define BOOST_THREAD_ONCE_HPP

//  once.hpp
//
//  (C) Copyright 2006-7 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "third_party/boost/boost/thread/detail/platform.hpp"
#if defined(BOOST_THREAD_PLATFORM_WIN32)
#include "third_party/boost/boost/thread/win32/once.hpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
#include "third_party/boost/boost/thread/pthread/once.hpp"
#else
#error "Boost threads unavailable on this platform"
#endif

#include "third_party/boost/boost/config/abi_prefix.hpp"

namespace boost
{
    inline void call_once(void (*func)(),once_flag& flag)
    {
        call_once(flag,func);
    }
}

#include "third_party/boost/boost/config/abi_suffix.hpp"

#endif
