/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
itl_ptime provides adapter code for boost::posix_time::ptime.
It implements incrementation (++) decrementation (--) and a neutral element
w.r.t. addition (identity_element()).
------------------------------------------------------------------------------*/

#ifndef BOOST_ICL_PTIME_HPP_JOFA_080416
#define BOOST_ICL_PTIME_HPP_JOFA_080416

#include "third_party/boost/boost/icl/detail/boost_config.hpp"
#include "third_party/boost/boost/detail/workaround.hpp"

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4100) // boost/date_time/time.hpp(80) : warning C4100: 'as_offset' : unreferenced formal parameter
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4244) // 'argument' : conversion from 'int' to 'unsigned short', possible loss of data
#pragma warning(disable:4702) // boost\lexical_cast.hpp(1159) : warning C4702: unreachable code
#pragma warning(disable:4996) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif                        

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "third_party/boost/boost/date_time/posix_time/posix_time.hpp"

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include "third_party/boost/boost/icl/type_traits/identity_element.hpp"
#include "third_party/boost/boost/icl/type_traits/difference_type_of.hpp"
#include "third_party/boost/boost/icl/type_traits/size_type_of.hpp"
#include "third_party/boost/boost/icl/type_traits/is_discrete.hpp"

namespace boost{namespace icl
{
    template<> struct is_discrete<boost::posix_time::ptime>
    {
        typedef is_discrete type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> 
    inline boost::posix_time::ptime identity_element<boost::posix_time::ptime>::value()
    { 
        return boost::posix_time::ptime(boost::posix_time::min_date_time); 
    }

    template<> 
    struct has_difference<boost::posix_time::ptime> 
    { 
        typedef has_difference type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };  

    template<> 
    struct difference_type_of<boost::posix_time::ptime> 
    { 
        typedef boost::posix_time::time_duration type; 
    };  

    template<> 
    struct size_type_of<boost::posix_time::ptime> 
    { 
        typedef boost::posix_time::time_duration type; 
    };  

    // ------------------------------------------------------------------------
    boost::posix_time::ptime operator ++(boost::posix_time::ptime& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::ptime operator --(boost::posix_time::ptime& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }

    // ------------------------------------------------------------------------
    template<> struct is_discrete<boost::posix_time::time_duration>
    {
        typedef is_discrete type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> 
    struct has_difference<boost::posix_time::time_duration> 
    { 
        typedef has_difference type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };  

    template<> 
    struct size_type_of<boost::posix_time::time_duration> 
    { 
        typedef boost::posix_time::time_duration type; 
    };  

    boost::posix_time::time_duration operator ++(boost::posix_time::time_duration& x)
    {
        return x += boost::posix_time::ptime::time_duration_type::unit();
    }

    boost::posix_time::time_duration operator --(boost::posix_time::time_duration& x)
    {
        return x -= boost::posix_time::ptime::time_duration_type::unit();
    }
}} // namespace icl boost

#endif


