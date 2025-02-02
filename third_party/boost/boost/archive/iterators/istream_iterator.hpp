#ifndef BOOST_ARCHIVE_ITERATORS_ISTREAM_ITERATOR_HPP
#define BOOST_ARCHIVE_ITERATORS_ISTREAM_ITERATOR_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// istream_iterator.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// note: this is a custom version of the standard istream_iterator.
// This is necessary as the standard version doesn't work as expected
// for wchar_t based streams on systems for which wchar_t not a true
// type but rather a synonym for some integer type.

#include <cstddef> // NULL
#include <istream>
#include "third_party/boost/boost/iterator/iterator_facade.hpp"

namespace boost { 
namespace archive {
namespace iterators {

// given a type, make an input iterator based on a pointer to that type
template<class Elem = char>
class istream_iterator :  
    public boost::iterator_facade<
        istream_iterator<Elem>,
        Elem,
        std::input_iterator_tag,
        Elem
    >
{
    friend class boost::iterator_core_access;
    typedef istream_iterator this_t ;
    typedef BOOST_DEDUCED_TYPENAME boost::iterator_facade<
        istream_iterator<Elem>,
        Elem,
        std::input_iterator_tag,
        Elem
    > super_t;
    typedef BOOST_DEDUCED_TYPENAME std::basic_istream<Elem> istream_type;
 
    //Access the value referred to 
    Elem dereference() const {
        return m_current_value;
    }

    bool equal(const this_t & rhs) const {
        // note: only  works for comparison against end of stream
        return m_istream == rhs.m_istream;
    }

    void increment(){
        if(NULL != m_istream){
            m_current_value = static_cast<Elem>(m_istream->get());
            if(! m_istream->good()){
                const_cast<this_t *>(this)->m_istream = NULL;
            }
        }
    }

    istream_type *m_istream;
    Elem m_current_value;
public:
    istream_iterator(istream_type & is) :
        m_istream(& is)
    {
        increment();
    }

    istream_iterator() :
        m_istream(NULL)
    {}

    istream_iterator(const istream_iterator<Elem> & rhs) :
        m_istream(rhs.m_istream),
        m_current_value(rhs.m_current_value)
    {}

};

} // namespace iterators
} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_ITERATORS_ISTREAM_ITERATOR_HPP
