#ifndef BOOST_THREAD_CONDITION_VARIABLE_HPP
#define BOOST_THREAD_CONDITION_VARIABLE_HPP

//  condition_variable.hpp
//
//  (C) Copyright 2007 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "third_party/boost/boost/thread/detail/platform.hpp"
#if defined(BOOST_THREAD_PLATFORM_WIN32)
#include "third_party/boost/boost/thread/win32/condition_variable.hpp"
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
#include "third_party/boost/boost/thread/pthread/condition_variable.hpp"
#else
#error "Boost threads unavailable on this platform"
#endif

#endif
