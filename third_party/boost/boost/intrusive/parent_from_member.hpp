/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2010-2010
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTRUSIVE_GET_PARENT_FROM_MEMBER_HPP
#define BOOST_INTRUSIVE_GET_PARENT_FROM_MEMBER_HPP

#include "third_party/boost/boost/intrusive/detail/config_begin.hpp"
#include "third_party/boost/boost/intrusive/detail/parent_from_member.hpp"

namespace boost {
namespace intrusive {

//! Given a pointer to a member and its corresponding pointer to data member,
//! this function returns the pointer of the parent containing that member.
//! Note: this function does not work with pointer to members that rely on 
//! virtual inheritance.
template<class Parent, class Member>
inline Parent *get_parent_from_member(Member *member, const Member Parent::* ptr_to_member)
{  return ::boost::intrusive::detail::parent_from_member(member, ptr_to_member);  }

//! Given a const pointer to a member and its corresponding const pointer to data member,
//! this function returns the const pointer of the parent containing that member.
//! Note: this function does not work with pointer to members that rely on 
//! virtual inheritance.
template<class Parent, class Member>
inline const Parent *get_parent_from_member(const Member *member, const Member Parent::* ptr_to_member)
{  return ::boost::intrusive::detail::parent_from_member(member, ptr_to_member);  }

}  //namespace intrusive {
}  //namespace boost {

#include "third_party/boost/boost/intrusive/detail/config_end.hpp"

#endif   //#ifndef BOOST_INTRUSIVE_GET_PARENT_FROM_MEMBER_HPP
