#ifndef BOOST_ARCHIVE_ITERATORS_UNESCAPE_HPP
#define BOOST_ARCHIVE_ITERATORS_UNESCAPE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// unescape.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "third_party/boost/boost/assert.hpp"

#include "third_party/boost/boost/config.hpp" // for BOOST_DEDUCED_TYPENAME
#include "third_party/boost/boost/iterator/iterator_adaptor.hpp"
//#include "third_party/boost/boost/iterator/iterator_traits.hpp"
#include "third_party/boost/boost/pointee.hpp"

namespace boost { 
namespace archive {
namespace iterators {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// class used by text archives to translate char strings to wchar_t
// strings of the currently selected locale
template<class Derived, class Base>
class unescape 
    : public boost::iterator_adaptor<
        unescape<Derived, Base>,
        Base, 
        BOOST_DEDUCED_TYPENAME pointee<Base>::type,
        single_pass_traversal_tag,
        BOOST_DEDUCED_TYPENAME pointee<Base>::type
    >
{
    friend class boost::iterator_core_access;
    typedef BOOST_DEDUCED_TYPENAME boost::iterator_adaptor<
        unescape<Derived, Base>, 
        Base, 
        BOOST_DEDUCED_TYPENAME pointee<Base>::type,
        single_pass_traversal_tag,
        BOOST_DEDUCED_TYPENAME pointee<Base>::type
    > super_t;

    typedef unescape<Derived, Base> this_t;
public:
    typedef BOOST_DEDUCED_TYPENAME this_t::value_type value_type;
    typedef BOOST_DEDUCED_TYPENAME this_t::reference reference;
private:
    value_type dereference_impl() {
        if(! m_full){
            m_current_value = static_cast<Derived *>(this)->drain();
            m_full = true;
        }
        return m_current_value;
    }

    reference dereference() const {
        return const_cast<this_t *>(this)->dereference_impl();
    }

    value_type m_current_value;
    bool m_full;

    void increment(){
        ++(this->base_reference());
        dereference_impl();
        m_full = false;
    };

public:

    unescape(Base base) : 
        super_t(base),
        m_full(false)
    {}

};

} // namespace iterators
} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_ITERATORS_UNESCAPE_HPP
