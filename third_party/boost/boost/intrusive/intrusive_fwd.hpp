/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007-2009
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_FWD_HPP
#define BOOST_INTRUSIVE_FWD_HPP

#include <cstddef>
#include "third_party/boost/boost/intrusive/link_mode.hpp"
#include "third_party/boost/boost/intrusive/detail/workaround.hpp"

/// @cond

namespace boost {

namespace intrusive {

struct none;

}  //namespace intrusive{
}  //namespace boost{

namespace boost {
namespace intrusive {

////////////////////////////
//     Node algorithms
////////////////////////////

//Algorithms predeclarations
template<class NodeTraits>
class circular_list_algorithms;

template<class NodeTraits>
class circular_slist_algorithms;

template<class NodeTraits>
class rbtree_algorithms;

////////////////////////////
//       Containers
////////////////////////////

//slist
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   , class O5  = none
   >
#else
template<class T, class ...Options>
#endif
class slist;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class slist_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class slist_member_hook;

//list
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class T, class ...Options>
#endif
class list;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class list_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class list_member_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class list_hook;

//rbtree/set/multiset
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class rbtree;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class multiset;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class set_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class set_member_hook;

//splaytree/splay_set/splay_multiset
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class splaytree;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class splay_set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class splay_multiset;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class splay_set_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class splay_set_member_hook;

//avltree/avl_set/avl_multiset
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class avltree;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class avl_set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class avl_multiset;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class avl_set_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class avl_set_member_hook;


//treap/treap_set/treap_multiset
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class treap;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class treap_set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class treap_multiset;

//Default priority comparison functor
template <class T>
struct priority_compare;

//sgtree/sg_set/sg_multiset
#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class sgtree;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class sg_set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class T, class ...Options>
#endif
class sg_multiset;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class bs_set_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class bs_set_member_hook;

//hashtable/unordered_set/unordered_multiset

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   , class O5  = none
   , class O6  = none
   , class O7  = none
   , class O8  = none
   , class O9  = none
   , class O10 = none
   >
#else
template<class T, class ...Options>
#endif
class hashtable;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   , class O5  = none
   , class O6  = none
   , class O7  = none
   , class O8  = none
   , class O9  = none
   , class O10 = none
   >
#else
template<class T, class ...Options>
#endif
class unordered_set;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class T
   , class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   , class O5  = none
   , class O6  = none
   , class O7  = none
   , class O8  = none
   , class O9  = none
   , class O10 = none
   >
#else
template<class T, class ...Options>
#endif
class unordered_multiset;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class unordered_set_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   , class O4  = none
   >
#else
template<class ...Options>
#endif
class unordered_set_member_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class any_base_hook;

#if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) && !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template
   < class O1  = none
   , class O2  = none
   , class O3  = none
   >
#else
template<class ...Options>
#endif
class any_member_hook;

}  //namespace intrusive {
}  //namespace boost {

/// @endcond

#endif   //#ifndef BOOST_INTRUSIVE_FWD_HPP
