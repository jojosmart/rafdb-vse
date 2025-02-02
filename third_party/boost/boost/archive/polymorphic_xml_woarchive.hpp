#ifndef BOOST_ARCHIVE_POLYMORPHIC_XML_WOARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_XML_WOARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// polymorphic_xml_oarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "third_party/boost/boost/config.hpp"
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include "third_party/boost/boost/archive/xml_woarchive.hpp"
#include "third_party/boost/boost/archive/detail/polymorphic_oarchive_route.hpp"

namespace boost { 
namespace archive {

typedef detail::polymorphic_oarchive_route<
        xml_woarchive_impl<naked_xml_woarchive> 
> polymorphic_xml_woarchive;

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(
    boost::archive::polymorphic_xml_woarchive
)

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_POLYMORPHIC_XML_WOARCHIVE_HPP

