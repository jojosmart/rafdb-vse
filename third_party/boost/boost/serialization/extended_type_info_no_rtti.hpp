#ifndef BOOST_EXTENDED_TYPE_INFO_NO_RTTI_HPP
#define BOOST_EXTENDED_TYPE_INFO_NO_RTTI_HPP

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

// extended_type_info_no_rtti.hpp: implementation for version that depends
// on runtime typing (rtti - typeid) but uses a user specified string
// as the portable class identifier.

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.
#include "third_party/boost/boost/assert.hpp"

#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/static_assert.hpp"

#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/type_traits/is_polymorphic.hpp"
#include "third_party/boost/boost/type_traits/remove_const.hpp"

#include "third_party/boost/boost/serialization/static_warning.hpp"
#include "third_party/boost/boost/serialization/singleton.hpp"
#include "third_party/boost/boost/serialization/extended_type_info.hpp"
#include "third_party/boost/boost/serialization/factory.hpp"
#include "third_party/boost/boost/serialization/throw_exception.hpp"

#include "third_party/boost/boost/serialization/config.hpp"
// hijack serialization access
#include "third_party/boost/boost/serialization/access.hpp"

#include "third_party/boost/boost/config/abi_prefix.hpp" // must be the last header
#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4251 4231 4660 4275 4511 4512)
#endif

namespace boost {
namespace serialization {
///////////////////////////////////////////////////////////////////////
// define a special type_info that doesn't depend on rtti which is not
// available in all situations.

namespace no_rtti_system {

// common base class to share type_info_key.  This is used to 
// identify the method used to keep track of the extended type
class BOOST_SERIALIZATION_DECL(BOOST_PP_EMPTY()) extended_type_info_no_rtti_0 : 
    public extended_type_info
{
protected:
    extended_type_info_no_rtti_0(const char * key);
    ~extended_type_info_no_rtti_0();
public:
    virtual bool
    is_less_than(const boost::serialization::extended_type_info &rhs) const ;
    virtual bool
    is_equal(const boost::serialization::extended_type_info &rhs) const ;
};

} // no_rtti_system

template<class T>
class extended_type_info_no_rtti : 
    public no_rtti_system::extended_type_info_no_rtti_0,
    public singleton<extended_type_info_no_rtti< T > >
{
    template<bool tf>
    struct action {
        struct defined {
            static const char * invoke(){
                return guid< T >();
            }
        };
        struct undefined {
            // if your program traps here - you failed to 
            // export a guid for this type.  the no_rtti
            // system requires export for types serialized
            // as pointers.
            BOOST_STATIC_ASSERT(0 == sizeof(T));
            static const char * invoke();
        };
        static const char * invoke(){
            typedef 
                BOOST_DEDUCED_TYPENAME boost::mpl::if_c<
                    tf,
                    defined,
                    undefined
                >::type type;
            return type::invoke();
        }
    };
public:
    extended_type_info_no_rtti() :
        no_rtti_system::extended_type_info_no_rtti_0(get_key())
    {
        key_register();
    }
    ~extended_type_info_no_rtti(){
        key_unregister();
    }
    const extended_type_info *
    get_derived_extended_type_info(const T & t) const {
        // find the type that corresponds to the most derived type.
        // this implementation doesn't depend on typeid() but assumes
        // that the specified type has a function of the following signature.
        // A common implemention of such a function is to define as a virtual
        // function. So if the is not a polymporphic type it's likely an error
        BOOST_STATIC_WARNING(boost::is_polymorphic< T >::value);
        const char * derived_key = t.get_key();
        BOOST_ASSERT(NULL != derived_key);
        return boost::serialization::extended_type_info::find(derived_key);
    }
    const char * get_key() const{
        return action<guid_defined< T >::value >::invoke();
    }
    virtual const char * get_debug_info() const{
        return action<guid_defined< T >::value >::invoke();
    }
    virtual void * construct(unsigned int count, ...) const{
        // count up the arguments
        std::va_list ap;
        va_start(ap, count);
        switch(count){
        case 0:
            return factory<BOOST_DEDUCED_TYPENAME boost::remove_const< T >::type, 0>(ap);
        case 1:
            return factory<BOOST_DEDUCED_TYPENAME boost::remove_const< T >::type, 1>(ap);
        case 2:
            return factory<BOOST_DEDUCED_TYPENAME boost::remove_const< T >::type, 2>(ap);
        case 3:
            return factory<BOOST_DEDUCED_TYPENAME boost::remove_const< T >::type, 3>(ap);
        case 4:
            return factory<BOOST_DEDUCED_TYPENAME boost::remove_const< T >::type, 4>(ap);
        default:
            BOOST_ASSERT(false); // too many arguments
            // throw exception here?
            return NULL;
        }
    }
    virtual void destroy(void const * const p) const{
        boost::serialization::access::destroy(
            static_cast<T const * const>(p)
        );
        //delete static_cast<T const * const>(p) ;
    }
};

} // namespace serialization
} // namespace boost

///////////////////////////////////////////////////////////////////////////////
// If no other implementation has been designated as default, 
// use this one.  To use this implementation as the default, specify it
// before any of the other headers.

#ifndef BOOST_SERIALIZATION_DEFAULT_TYPE_INFO
    #define BOOST_SERIALIZATION_DEFAULT_TYPE_INFO
    namespace boost {
    namespace serialization {
    template<class T>
    struct extended_type_info_impl {
        typedef BOOST_DEDUCED_TYPENAME 
            boost::serialization::extended_type_info_no_rtti< T > type;
    };
    } // namespace serialization
    } // namespace boost
#endif

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif
#include "third_party/boost/boost/config/abi_suffix.hpp" // pops abi_suffix.hpp pragmas

#endif // BOOST_EXTENDED_TYPE_INFO_NO_RTTI_HPP
