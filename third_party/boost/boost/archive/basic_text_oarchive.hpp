#ifndef BOOST_ARCHIVE_BASIC_TEXT_OARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_TEXT_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_text_oarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// archives stored as text - note these ar templated on the basic
// stream templates to accommodate wide (and other?) kind of characters
//
// note the fact that on libraries without wide characters, ostream is
// is not a specialization of basic_ostream which in fact is not defined
// in such cases.   So we can't use basic_ostream<OStream::char_type> but rather
// use two template parameters

#include "third_party/boost/boost/assert.hpp"
#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/serialization/pfto.hpp"
#include "third_party/boost/boost/detail/workaround.hpp"

#include "third_party/boost/boost/archive/detail/common_oarchive.hpp"
#include "third_party/boost/boost/serialization/string.hpp"

#include "third_party/boost/boost/archive/detail/abi_prefix.hpp" // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

/////////////////////////////////////////////////////////////////////////
// class basic_text_oarchive 
template<class Archive>
class basic_text_oarchive : 
    public detail::common_oarchive<Archive>
{
protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) \
|| BOOST_WORKAROUND(__BORLANDC__,BOOST_TESTED_AT(0x560))
public:
#elif defined(BOOST_MSVC)
    // for some inexplicable reason insertion of "class" generates compile erro
    // on msvc 7.1
    friend detail::interface_oarchive<Archive>;
#else
    friend class detail::interface_oarchive<Archive>;
#endif
    enum {
        none,
        eol,
        space
    } delimiter;

    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    newtoken();

    void newline(){
        delimiter = eol;
    }

    // default processing - kick back to base class.  Note the
    // extra stuff to get it passed borland compilers
    typedef detail::common_oarchive<Archive> detail_common_oarchive;
    template<class T>
    void save_override(T & t, BOOST_PFTO int){
        this->detail_common_oarchive::save_override(t, 0);
    }

    // start new objects on a new line
    void save_override(const object_id_type & t, int){
        this->This()->newline();
        this->detail_common_oarchive::save_override(t, 0);
    }

    // text file don't include the optional information 
    void save_override(const class_id_optional_type & /* t */, int){}

    void save_override(const class_name_type & t, int){
        const std::string s(t);
        * this->This() << s;
    }

    BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
    init();

    basic_text_oarchive(unsigned int flags) :
        detail::common_oarchive<Archive>(flags),
        delimiter(none)
    {}
    ~basic_text_oarchive(){}
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include "third_party/boost/boost/archive/detail/abi_suffix.hpp" // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_BASIC_TEXT_OARCHIVE_HPP
