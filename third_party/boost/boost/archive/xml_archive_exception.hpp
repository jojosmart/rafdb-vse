#ifndef BOOST_ARCHIVE_XML_ARCHIVE_EXCEPTION_HPP
#define BOOST_ARCHIVE_XML_ARCHIVE_EXCEPTION_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_archive_exception.hpp:

// (C) Copyright 2007 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <exception>
#include "third_party/boost/boost/assert.hpp"

#include "third_party/boost/boost/config.hpp" 
#include "third_party/boost/boost/preprocessor/empty.hpp"
#include "third_party/boost/boost/archive/detail/decl.hpp"
#include "third_party/boost/boost/archive/archive_exception.hpp"

#include "third_party/boost/boost/archive/detail/abi_prefix.hpp" // must be the last header

namespace boost {
namespace archive {

//////////////////////////////////////////////////////////////////////
// exceptions thrown by xml archives
//
class BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) xml_archive_exception : 
    public virtual boost::archive::archive_exception
{
public:
    typedef enum {
        xml_archive_parsing_error,    // see save_register
        xml_archive_tag_mismatch,
        xml_archive_tag_name_error
    } exception_code;
    xml_archive_exception(
        exception_code c, 
        const char * e1 = NULL,
        const char * e2 = NULL
    );
};

}// namespace archive
}// namespace boost

#include "third_party/boost/boost/archive/detail/abi_suffix.hpp" // pops abi_suffix.hpp pragmas

#endif //BOOST_XML_ARCHIVE_ARCHIVE_EXCEPTION_HPP
