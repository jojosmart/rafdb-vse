#ifndef BOOST_SERIALIZATION_EXTENDED_TYPE_INFO_HPP
#define BOOST_SERIALIZATION_EXTENDED_TYPE_INFO_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// extended_type_info.hpp: interface for portable version of type_info

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// for now, extended type info is part of the serialization libraries
// this could change in the future.
#include <cstdarg>
#include "third_party/boost/boost/assert.hpp"
#include <cstddef> // NULL
#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/noncopyable.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"

#include "third_party/boost/boost/serialization/config.hpp"
#include "third_party/boost/boost/config/abi_prefix.hpp" // must be the last header
#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4251 4231 4660 4275)
#endif

#define BOOST_SERIALIZATION_MAX_KEY_SIZE 128

namespace boost { 
namespace serialization {

namespace void_cast_detail{
    class void_caster;
}

class BOOST_SERIALIZATION_DECL(BOOST_PP_EMPTY()) extended_type_info :
    private boost::noncopyable
{
private:
    friend class boost::serialization::void_cast_detail::void_caster;

    // used to uniquely identify the type of class derived from this one
    // so that different derivations of this class can be simultaneously
    // included in implementation of sets and maps.
    const unsigned int m_type_info_key;
    virtual bool is_less_than(const extended_type_info & /*rhs*/) const = 0;
    virtual bool is_equal(const extended_type_info & /*rhs*/) const = 0;
    const char * m_key;

protected:
    void key_unregister() const;
    void key_register() const;
    // this class can't be used as is. It's just the 
    // common functionality for all type_info replacement
    // systems.  Hence, make these protected
    extended_type_info(
        const unsigned int type_info_key,
        const char * key
    );
    // account for bogus gcc warning
    #if defined(__GNUC__)
    virtual
    #endif
    ~extended_type_info();
public:
    const char * get_key() const {
        return m_key;
    }
    virtual const char * get_debug_info() const = 0;
    bool operator<(const extended_type_info &rhs) const;
    bool operator==(const extended_type_info &rhs) const;
    bool operator!=(const extended_type_info &rhs) const {
        return !(operator==(rhs));
    }
    // note explicit "export" of static function to work around
    // gcc 4.5 mingw error
    static const extended_type_info *
    find(const char *key);
    // for plugins
    virtual void * construct(unsigned int /*count*/ = 0, ...) const = 0;
    virtual void destroy(void const * const /*p*/) const = 0;
};

template<class T>
struct guid_defined : boost::mpl::false_ {};

namespace ext {
    template <typename T>
    struct guid_impl
    {
        static inline const char * call()
        {
            return NULL;
        }
    };
}

template<class T>
inline const char * guid(){
    return ext::guid_impl<T>::call();
}

} // namespace serialization 
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include "third_party/boost/boost/config/abi_suffix.hpp" // pops abi_suffix.hpp pragmas

#endif // BOOST_SERIALIZATION_EXTENDED_TYPE_INFO_HPP
