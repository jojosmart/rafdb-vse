#ifndef BOOST_ARCHIVE_XML_WIARCHIVE_HPP
#define BOOST_ARCHIVE_XML_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_wiarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "third_party/boost/boost/config.hpp"
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <istream>

//#include "third_party/boost/boost/scoped_ptr.hpp"
#include "third_party/boost/boost/archive/detail/auto_link_warchive.hpp"
#include "third_party/boost/boost/archive/basic_text_iprimitive.hpp"
#include "third_party/boost/boost/archive/basic_xml_iarchive.hpp"
#include "third_party/boost/boost/archive/detail/register_archive.hpp"
#include "third_party/boost/boost/serialization/item_version_type.hpp"

#include "third_party/boost/boost/archive/detail/abi_prefix.hpp" // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace boost { 
namespace archive {

template<class CharType>
class basic_xml_grammar;
typedef basic_xml_grammar<wchar_t> xml_wgrammar;

template<class Archive>
class xml_wiarchive_impl : 
    public basic_text_iprimitive<std::wistream>,
    public basic_xml_iarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class detail::interface_iarchive<Archive>;
    friend class basic_xml_iarchive<Archive>;
    friend class load_access;
protected:
#endif
    // instances of micro xml parser to parse start preambles
    // scoped_ptr doesn't play nice with borland - so use a naked pointer
    // scoped_ptr<xml_wgrammar> gimpl;
    xml_wgrammar *gimpl;
    std::wistream & get_is(){
        return is;
    }
    template<class T>
    void 
    load(T & t){
        basic_text_iprimitive<std::wistream>::load(t);
    }
    void 
    load(version_type & t){
        unsigned int v;
        load(v);
        t = version_type(v);
    }
    void 
    load(boost::serialization::item_version_type & t){
        unsigned int v;
        load(v);
        t = boost::serialization::item_version_type(v);
    }
    BOOST_WARCHIVE_DECL(void)
    load(char * t);
    #ifndef BOOST_NO_INTRINSIC_WCHAR_T
    BOOST_WARCHIVE_DECL(void)
    load(wchar_t * t);
    #endif
    BOOST_WARCHIVE_DECL(void)
    load(std::string &s);
    #ifndef BOOST_NO_STD_WSTRING
    BOOST_WARCHIVE_DECL(void)
    load(std::wstring &ws);
    #endif
    template<class T>
    void load_override(T & t, BOOST_PFTO int){
        basic_xml_iarchive<Archive>::load_override(t, 0);
    }
    BOOST_WARCHIVE_DECL(void)
    load_override(class_name_type & t, int);
    BOOST_WARCHIVE_DECL(void) 
    init();
    BOOST_WARCHIVE_DECL(BOOST_PP_EMPTY()) 
    xml_wiarchive_impl(std::wistream & is, unsigned int flags) ;
    BOOST_WARCHIVE_DECL(BOOST_PP_EMPTY()) 
    ~xml_wiarchive_impl();
};

// do not derive from the classes below.  If you want to extend this functionality
// via inhertance, derived from xml_wiarchive_impl instead.  This will
// preserve correct static polymorphism.

// same as xml_wiarchive below - without the shared_ptr_helper
class naked_xml_wiarchive : 
    public xml_wiarchive_impl<naked_xml_wiarchive>
{
public:
    naked_xml_wiarchive(std::wistream & is, unsigned int flags = 0) :
        xml_wiarchive_impl<naked_xml_wiarchive>(is, flags)
    {}
    ~naked_xml_wiarchive(){}
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#  pragma warning(pop) 
#endif

#include "third_party/boost/boost/archive/detail/abi_suffix.hpp" // pops abi_suffix.hpp pragmas

// note special treatment of shared_ptr. This type needs a special
// structure associated with every archive.  We created a "mix-in"
// class to provide this functionality.  Since shared_ptr holds a
// special esteem in the boost library - we included it here by default.
#include "third_party/boost/boost/archive/shared_ptr_helper.hpp"

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace boost { 
namespace archive {

class xml_wiarchive : 
    public xml_wiarchive_impl<xml_wiarchive>,
    public detail::shared_ptr_helper
{
public:
    xml_wiarchive(std::wistream & is, unsigned int flags = 0) :
        xml_wiarchive_impl<xml_wiarchive>(is, flags)
    {}
    ~xml_wiarchive(){}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::xml_wiarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_XML_WIARCHIVE_HPP
