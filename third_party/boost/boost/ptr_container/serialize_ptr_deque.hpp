// Copyright Sebastian Ramacher, 2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PTR_CONTAINER_SERIALIZE_PTR_DEQUE_HPP
#define BOOST_PTR_CONTAINER_SERIALIZE_PTR_DEQUE_HPP

#include "third_party/boost/boost/ptr_container/detail/serialize_reversible_cont.hpp"
#include "third_party/boost/boost/ptr_container/ptr_deque.hpp"

namespace boost 
{

namespace serialization 
{

template<class Archive, class T, class CloneAllocator, class Allocator>
void serialize(Archive& ar, ptr_deque<T, CloneAllocator, Allocator>& c, const unsigned int version)
{
   split_free(ar, c, version);
}

} // namespace serialization
} // namespace boost

#endif
