//  Boost string_algo library string_traits.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_STD_ROPE_TRAITS_HPP
#define BOOST_STRING_STD_ROPE_TRAITS_HPP

#include "third_party/boost/boost/algorithm/string/yes_no_type.hpp"
#include <rope>
#include "third_party/boost/boost/algorithm/string/sequence_traits.hpp"

namespace boost {
    namespace algorithm {

//  SGI's std::rope<> traits  -----------------------------------------------//

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        // native replace tester
        template<typename T, typename TraitsT, typename AllocT>
        yes_type has_native_replace_tester( const std::rope<T, TraitsT, AllocT>* );

        // stable iterators tester
        template<typename T, typename TraitsT, typename AllocT>
        yes_type has_stable_iterators_tester( const std::rope<T, TraitsT, AllocT>* );

        // const time insert tester
        template<typename T, typename TraitsT, typename AllocT>
        yes_type has_const_time_insert_tester( const std::rope<T, TraitsT, AllocT>* );

        // const time erase tester
        template<typename T, typename TraitsT, typename AllocT>
        yes_type has_const_time_erase_tester( const std::rope<T, TraitsT, AllocT>* );

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    
    // native replace trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_native_replace< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            BOOST_STATIC_CONSTANT(bool, value=true);
#endif // BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // stable iterators trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_stable_iterators< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            BOOST_STATIC_CONSTANT(bool, value=true);
#endif // BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // const time insert trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_const_time_insert< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            BOOST_STATIC_CONSTANT(bool, value=true);
#endif // BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };

    // const time erase trait
        template<typename T, typename TraitsT, typename AllocT>
        class has_const_time_erase< std::rope<T,TraitsT,AllocT> >
        {
        public:
#if BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            enum { value = true };
#else
            BOOST_STATIC_CONSTANT(bool, value=true);
#endif // BOOST_WORKAROUND( __IBMCPP__, <= 600 )
            typedef mpl::bool_<value> type;     
        };
#endif


    } // namespace algorithm
} // namespace boost


#endif  // BOOST_STRING_ROPE_TRAITS_HPP
