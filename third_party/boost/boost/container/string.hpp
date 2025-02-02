//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1996,1997
// Silicon Graphics Computer Systems, Inc.
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Silicon Graphics makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.
//
//
// Copyright (c) 1994
// Hewlett-Packard Company
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hewlett-Packard Company makes no
// representations about the suitability of this software for any
// purpose.  It is provided "as is" without express or implied warranty.

#ifndef BOOST_CONTAINER_STRING_HPP
#define BOOST_CONTAINER_STRING_HPP

#include "third_party/boost/boost/container/detail/config_begin.hpp"
#include "third_party/boost/boost/container/detail/workaround.hpp"

#include "third_party/boost/boost/container/detail/workaround.hpp"
#include "third_party/boost/boost/container/container_fwd.hpp"
#include "third_party/boost/boost/container/detail/utilities.hpp"
#include "third_party/boost/boost/container/detail/iterators.hpp"
#include "third_party/boost/boost/container/detail/algorithms.hpp"
#include "third_party/boost/boost/container/detail/version_type.hpp"
#include "third_party/boost/boost/container/detail/allocation_type.hpp"
#include "third_party/boost/boost/container/allocator/allocator_traits.hpp"
#include "third_party/boost/boost/container/detail/mpl.hpp"
#include "third_party/boost/boost/move/move.hpp"
#include "third_party/boost/boost/static_assert.hpp"

#include <functional>
#include <string>
#include <stdexcept>      
#include <utility>  
#include <iterator>
#include <memory>
#include <algorithm>
#include <iosfwd>
#include <istream>
#include <ostream>
#include <ios>
#include <locale>
#include <cstddef>
#include <climits>
#include "third_party/boost/boost/container/detail/type_traits.hpp"
#include "third_party/boost/boost/detail/no_exceptions_support.hpp"
#include "third_party/boost/boost/type_traits/has_trivial_destructor.hpp"
#include "third_party/boost/boost/aligned_storage.hpp"

#ifdef BOOST_CONTAINER_DOXYGEN_INVOKED
namespace boost {
namespace container {
#else
namespace boost {
namespace container {
#endif

/// @cond
namespace container_detail {
// ------------------------------------------------------------
// Class basic_string_base.  

// basic_string_base is a helper class that makes it it easier to write
// an exception-safe version of basic_string.  The constructor allocates,
// but does not initialize, a block of memory.  The destructor
// deallocates, but does not destroy elements within, a block of
// memory.  The destructor assumes that the memory either is the internal buffer, 
// or else points to a block of memory that was allocated using _String_base's 
// allocator and whose size is this->m_storage.
template <class A>
class basic_string_base
{
   BOOST_MOVABLE_BUT_NOT_COPYABLE(basic_string_base)

   typedef allocator_traits<A> allocator_traits_type;
 public:
   typedef A allocator_type;
   //! The stored allocator type
   typedef allocator_type                          stored_allocator_type;
   typedef typename allocator_traits_type::pointer     pointer;
   typedef typename allocator_traits_type::value_type  value_type;
   typedef typename allocator_traits_type::size_type   size_type;

   basic_string_base()
      : members_()
   {  init(); }

   basic_string_base(const allocator_type& a)
      : members_(a)
   {  init(); }

   basic_string_base(const allocator_type& a, size_type n)
      : members_(a)
   {  
      this->init(); 
      this->allocate_initial_block(n);
   }

   basic_string_base(BOOST_RV_REF(basic_string_base) b)
      :  members_(boost::move(b.alloc()))
   {  
      this->init();
      this->swap_data(b); 
   }

   ~basic_string_base() 
   {  
      if(!this->is_short()){
         this->deallocate_block(); 
         allocator_traits_type::destroy
            ( this->alloc()
            , static_cast<long_t*>(static_cast<void*>(&this->members_.m_repr.r))
            );
      }
   }

   private:

   //This is the structure controlling a long string 
   struct long_t
   {
      size_type      is_short  : 1;
      size_type      length    : (sizeof(size_type)*CHAR_BIT - 1);
      size_type      storage;
      pointer        start;

      long_t()
      {}

      long_t(const long_t &other)
      {
         this->is_short = other.is_short;
         length   = other.length;
         storage  = other.storage;
         start    = other.start;
      }

      long_t &operator =(const long_t &other)
      {
         this->is_short = other.is_short;
         length   = other.length;
         storage  = other.storage;
         start    = other.start;
         return *this;
      }
   };

   //This type is the first part of the structure controlling a short string
   //The "data" member stores
   struct short_header
   {
      unsigned char  is_short  : 1;
      unsigned char  length    : (CHAR_BIT - 1);
   };

   //This type has the same alignment and size as long_t but it's POD
   //so, unlike long_t, it can be placed in a union
   
   typedef typename boost::aligned_storage< sizeof(long_t), 
       container_detail::alignment_of<long_t>::value>::type   long_raw_t;

   protected:
   static const size_type  MinInternalBufferChars = 8;
   static const size_type  AlignmentOfValueType =
      alignment_of<value_type>::value;
   static const size_type  ShortDataOffset =
      container_detail::ct_rounded_size<sizeof(short_header),  AlignmentOfValueType>::value;
   static const size_type  ZeroCostInternalBufferChars =
      (sizeof(long_t) - ShortDataOffset)/sizeof(value_type);
   static const size_type  UnalignedFinalInternalBufferChars = 
      (ZeroCostInternalBufferChars > MinInternalBufferChars) ?
                ZeroCostInternalBufferChars : MinInternalBufferChars;

   struct short_t
   {
      short_header   h; 
      value_type     data[UnalignedFinalInternalBufferChars];
   };

   union repr_t
   {
      long_raw_t  r;
      short_t     s;

      short_t &short_repr() const
      {  return *const_cast<short_t *>(&s);  }

      long_t &long_repr() const
      {  return *const_cast<long_t*>(reinterpret_cast<const long_t*>(&r));  }
   };

   struct members_holder
      :  public A
   {
      members_holder()
         : A()
      {}

      template<class AllocatorConvertible>
      explicit members_holder(BOOST_FWD_REF(AllocatorConvertible) a)
         :  A(boost::forward<AllocatorConvertible>(a))
      {}

      repr_t m_repr;
   } members_;

   const A &alloc() const
   {  return members_;  }

   A &alloc()
   {  return members_;  }

   static const size_type InternalBufferChars = (sizeof(repr_t) - ShortDataOffset)/sizeof(value_type);

   private:

   static const size_type MinAllocation = InternalBufferChars*2;

   protected:
   bool is_short() const
   {  return static_cast<bool>(this->members_.m_repr.s.h.is_short != 0);  }

   void is_short(bool yes)
   {  
      if(yes && !this->is_short()){
         allocator_traits_type::destroy
            ( this->alloc()
            , static_cast<long_t*>(static_cast<void*>(&this->members_.m_repr.r))
            );
      }
      else{
         allocator_traits_type::construct
            ( this->alloc()
            , static_cast<long_t*>(static_cast<void*>(&this->members_.m_repr.r))
            );
      }
      this->members_.m_repr.s.h.is_short = yes;
   }

   private:
   void init()
   {
      this->members_.m_repr.s.h.is_short = 1;
      this->members_.m_repr.s.h.length   = 0;
   }

   protected:

   typedef container_detail::integral_constant<unsigned, 1>      allocator_v1;
   typedef container_detail::integral_constant<unsigned, 2>      allocator_v2;
   typedef container_detail::integral_constant<unsigned,
      boost::container::container_detail::version<A>::value> alloc_version;

   std::pair<pointer, bool>
      allocation_command(allocation_type command,
                         size_type limit_size, 
                         size_type preferred_size,
                         size_type &received_size, pointer reuse = 0)
   {
      if(this->is_short() && (command & (expand_fwd | expand_bwd)) ){
         reuse = pointer(0);
         command &= ~(expand_fwd | expand_bwd);
      }
      return this->allocation_command
         (command, limit_size, preferred_size, received_size, reuse, alloc_version());
   }

   std::pair<pointer, bool>
      allocation_command(allocation_type command,
                         size_type limit_size, 
                         size_type preferred_size,
                         size_type &received_size,
                         const pointer &reuse,
                         allocator_v1)
   {
      (void)limit_size;
      (void)reuse;
      if(!(command & allocate_new))
         return std::pair<pointer, bool>(pointer(0), false);
      received_size = preferred_size;
      return std::make_pair(this->alloc().allocate(received_size), false);
   }

   std::pair<pointer, bool>
      allocation_command(allocation_type command,
                         size_type limit_size, 
                         size_type preferred_size,
                         size_type &received_size,
                         pointer reuse,
                         allocator_v2)
   {
      return this->alloc().allocation_command(command, limit_size, preferred_size, 
                                              received_size, reuse);
   }

   size_type next_capacity(size_type additional_objects) const
   {  return get_next_capacity(allocator_traits_type::max_size(this->alloc()), this->priv_storage(), additional_objects);  }

   void deallocate(pointer p, size_type n) 
   {  
      if (p && (n > InternalBufferChars))
         this->alloc().deallocate(p, n);
   }

   void construct(pointer p, const value_type &value = value_type())
   {
      allocator_traits_type::construct
         ( this->alloc()
         , container_detail::to_raw_pointer(p)
         , value
         );
   }

   void destroy(pointer p, size_type n)
   {
      for(; n--; ++p){
         allocator_traits_type::destroy
            ( this->alloc()
            , container_detail::to_raw_pointer(p)
            );
      }
   }

   void destroy(pointer p)
   {
      allocator_traits_type::destroy
         ( this->alloc()
         , container_detail::to_raw_pointer(p)
         );
   }

   void allocate_initial_block(size_type n)
   {
      if (n <= this->max_size()) {
         if(n > InternalBufferChars){
            size_type new_cap = this->next_capacity(n);
            pointer p = this->allocation_command(allocate_new, n, new_cap, new_cap).first;
            this->is_short(false);
            this->priv_long_addr(p);
            this->priv_size(0);
            this->priv_storage(new_cap);
         }
      }
      else
         throw_length_error();
   }

   void deallocate_block() 
   {  this->deallocate(this->priv_addr(), this->priv_storage());  }
      
   size_type max_size() const
   {  return allocator_traits_type::max_size(this->alloc()) - 1; }

   // Helper functions for exception handling.
   void throw_length_error() const
   {  throw(std::length_error("basic_string"));  }

   void throw_out_of_range() const
   {  throw(std::out_of_range("basic_string"));  }

   protected:
   size_type priv_capacity() const
   { return this->priv_storage() - 1; }

   pointer priv_short_addr() const
   {  return pointer(&this->members_.m_repr.short_repr().data[0]);  }

   pointer priv_long_addr() const
   {  return this->members_.m_repr.long_repr().start;  }

   pointer priv_addr() const
   {  return this->is_short() ? pointer(&this->members_.m_repr.short_repr().data[0]) : this->members_.m_repr.long_repr().start;  }

   void priv_long_addr(pointer addr)
   {  this->members_.m_repr.long_repr().start = addr;  }

   size_type priv_storage() const
   {  return this->is_short() ? priv_short_storage() : priv_long_storage();  }

   size_type priv_short_storage() const
   {  return InternalBufferChars;  }

   size_type priv_long_storage() const
   {  return this->members_.m_repr.long_repr().storage;  }

   void priv_storage(size_type storage)
   {  
      if(!this->is_short())
         this->priv_long_storage(storage);
   }

   void priv_long_storage(size_type storage)
   {  
      this->members_.m_repr.long_repr().storage = storage;
   }

   size_type priv_size() const
   {  return this->is_short() ? priv_short_size() : priv_long_size();  }

   size_type priv_short_size() const
   {  return this->members_.m_repr.short_repr().h.length;  }

   size_type priv_long_size() const
   {  return this->members_.m_repr.long_repr().length;  }

   void priv_size(size_type sz)
   {  
      if(this->is_short())
         this->priv_short_size(sz);
      else
         this->priv_long_size(sz);
   }

   void priv_short_size(size_type sz)
   {  
      this->members_.m_repr.s.h.length = (unsigned char)sz;
   }

   void priv_long_size(size_type sz)
   {  
      this->members_.m_repr.long_repr().length = static_cast<typename allocator_traits_type::size_type>(sz);
   }

   void swap_data(basic_string_base& other)
   {
      if(this->is_short()){
         if(other.is_short()){
            container_detail::do_swap(this->members_.m_repr, other.members_.m_repr);
         }
         else{
            repr_t copied(this->members_.m_repr);
            this->members_.m_repr.long_repr() = other.members_.m_repr.long_repr();
            other.members_.m_repr = copied;
         }
      }
      else{
         if(other.is_short()){
            repr_t copied(other.members_.m_repr);
            other.members_.m_repr.long_repr() = this->members_.m_repr.long_repr();
            this->members_.m_repr = copied;
         }
         else{
            container_detail::do_swap(this->members_.m_repr.long_repr(), other.members_.m_repr.long_repr());
         }
      }
   }
};

}  //namespace container_detail {

/// @endcond

//! The basic_string class represents a Sequence of characters. It contains all the 
//! usual operations of a Sequence, and, additionally, it contains standard string 
//! operations such as search and concatenation.
//!
//! The basic_string class is parameterized by character type, and by that type's 
//! Character Traits.
//! 
//! This class has performance characteristics very much like vector<>, meaning, 
//! for example, that it does not perform reference-count or copy-on-write, and that
//! concatenation of two strings is an O(N) operation. 
//! 
//! Some of basic_string's member functions use an unusual method of specifying positions 
//! and ranges. In addition to the conventional method using iterators, many of 
//! basic_string's member functions use a single value pos of type size_type to represent a 
//! position (in which case the position is begin() + pos, and many of basic_string's 
//! member functions use two values, pos and n, to represent a range. In that case pos is 
//! the beginning of the range and n is its size. That is, the range is 
//! [begin() + pos, begin() + pos + n). 
//! 
//! Note that the C++ standard does not specify the complexity of basic_string operations. 
//! In this implementation, basic_string has performance characteristics very similar to 
//! those of vector: access to a single character is O(1), while copy and concatenation 
//! are O(N).
//! 
//! In this implementation, begin(), 
//! end(), rbegin(), rend(), operator[], c_str(), and data() do not invalidate iterators.
//! In this implementation, iterators are only invalidated by member functions that
//! explicitly change the string's contents. 
#ifdef BOOST_CONTAINER_DOXYGEN_INVOKED
template <class CharT, class Traits = std::char_traits<CharT>, class A = std::allocator<CharT> >
#else
template <class CharT, class Traits, class A>
#endif
class basic_string
   :  private container_detail::basic_string_base<A> 
{
   /// @cond
   private:
   typedef allocator_traits<A> allocator_traits_type;
   BOOST_COPYABLE_AND_MOVABLE(basic_string)
   typedef container_detail::basic_string_base<A> base_t;
   static const typename base_t::size_type InternalBufferChars = base_t::InternalBufferChars;

   protected:
   // A helper class to use a char_traits as a function object.

   template <class Tr>
   struct Eq_traits
      : public std::binary_function<typename Tr::char_type,
                                    typename Tr::char_type,
                                    bool>
   {
      bool operator()(const typename Tr::char_type& x,
                      const typename Tr::char_type& y) const
         { return Tr::eq(x, y); }
   };

   template <class Tr>
   struct Not_within_traits
      : public std::unary_function<typename Tr::char_type, bool>
   {
      typedef const typename Tr::char_type* Pointer;
      const Pointer m_first;
      const Pointer m_last;

      Not_within_traits(Pointer f, Pointer l) 
         : m_first(f), m_last(l) {}

      bool operator()(const typename Tr::char_type& x) const 
      {
         return std::find_if(m_first, m_last, 
                        std::bind1st(Eq_traits<Tr>(), x)) == m_last;
      }
   };
   /// @endcond

   public:

   //! The allocator type
   typedef A                                       allocator_type;
   //! The stored allocator type
   typedef allocator_type                          stored_allocator_type;
   //! The type of object, CharT, stored in the string
   typedef CharT                                   value_type;
   //! The second template parameter Traits
   typedef Traits                                  traits_type;
   //! Pointer to CharT
   typedef typename allocator_traits_type::pointer pointer;
   //! Const pointer to CharT 
   typedef typename allocator_traits_type::const_pointer               const_pointer;
   //! Reference to CharT 
   typedef typename allocator_traits_type::reference                   reference;
   //! Const reference to CharT 
   typedef typename allocator_traits_type::const_reference             const_reference;
   //! An unsigned integral type
   typedef typename allocator_traits_type::size_type                   size_type;
   //! A signed integral type
   typedef typename allocator_traits_type::difference_type             difference_type;
   //! Iterator used to iterate through a string. It's a Random Access Iterator
   typedef pointer                                 iterator;
   //! Const iterator used to iterate through a string. It's a Random Access Iterator
   typedef const_pointer                           const_iterator;
   //! Iterator used to iterate backwards through a string
   typedef std::reverse_iterator<iterator>       reverse_iterator;
   //! Const iterator used to iterate backwards through a string
   typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
   //! The largest possible value of type size_type. That is, size_type(-1). 
   static const size_type npos;

   /// @cond
   private:
   typedef constant_iterator<CharT, difference_type> cvalue_iterator;
   typedef typename base_t::allocator_v1  allocator_v1;
   typedef typename base_t::allocator_v2  allocator_v2;
   typedef typename base_t::alloc_version  alloc_version;
   /// @endcond

   public:                         // Constructor, destructor, assignment.
   /// @cond
   struct reserve_t {};

   basic_string(reserve_t, size_type n,
                const allocator_type& a = allocator_type())
      //Select allocator as in copy constructor as reserve_t-based constructors
      //are two step copies optimized for capacity
      : base_t( allocator_traits_type::select_on_container_copy_construction(a)
              , n + 1)
   { this->priv_terminate_string(); }

   /// @endcond

   //! <b>Effects</b>: Default constructs a basic_string.
   //! 
   //! <b>Throws</b>: If allocator_type's default constructor throws.
   basic_string()
      : base_t()
   { this->priv_terminate_string(); }


   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter.
   //! 
   //! <b>Throws</b>: If allocator_type's copy constructor throws.
   explicit basic_string(const allocator_type& a)
      : base_t(a)
   { this->priv_terminate_string(); }

   //! <b>Effects</b>: Copy constructs a basic_string.
   //!
   //! <b>Postcondition</b>: x == *this.
   //! 
   //! <b>Throws</b>: If allocator_type's default constructor or copy constructor throws.
   basic_string(const basic_string& s) 
      :  base_t(allocator_traits_type::select_on_container_copy_construction(s.alloc()))
   { this->priv_range_initialize(s.begin(), s.end()); }

   //! <b>Effects</b>: Move constructor. Moves mx's resources to *this.
   //!
   //! <b>Throws</b>: If allocator_type's copy constructor throws.
   //! 
   //! <b>Complexity</b>: Constant.
   basic_string(BOOST_RV_REF(basic_string) s) 
      : base_t(boost::move((base_t&)s))
   {}

   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter,
   //!   and is initialized by a specific number of characters of the s string. 
   basic_string(const basic_string& s, size_type pos, size_type n = npos,
               const allocator_type& a = allocator_type()) 
      : base_t(a) 
   {
      if (pos > s.size())
         this->throw_out_of_range();
      else
         this->priv_range_initialize
            (s.begin() + pos, s.begin() + pos + container_detail::min_value(n, s.size() - pos));
   }

   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter,
   //!   and is initialized by a specific number of characters of the s c-string.
   basic_string(const CharT* s, size_type n,
               const allocator_type& a = allocator_type()) 
      : base_t(a) 
   { this->priv_range_initialize(s, s + n); }

   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter,
   //!   and is initialized by the null-terminated s c-string.
   basic_string(const CharT* s,
                const allocator_type& a = allocator_type())
      : base_t(a) 
   { this->priv_range_initialize(s, s + Traits::length(s)); }

   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter,
   //!   and is initialized by n copies of c.
   basic_string(size_type n, CharT c,
                const allocator_type& a = allocator_type())
      : base_t(a)
   {  
      this->priv_range_initialize(cvalue_iterator(c, n),
                                  cvalue_iterator());
   }

   //! <b>Effects</b>: Constructs a basic_string taking the allocator as parameter,
   //!   and a range of iterators.
   template <class InputIterator>
   basic_string(InputIterator f, InputIterator l,
               const allocator_type& a = allocator_type())
      : base_t(a)
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = container_detail::is_convertible<InputIterator, size_type>::value;
      typedef container_detail::bool_<aux_boolean> Result;
      this->priv_initialize_dispatch(f, l, Result());
   }

   //! <b>Effects</b>: Destroys the basic_string. All used memory is deallocated.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   ~basic_string() 
   {}
      
   //! <b>Effects</b>: Copy constructs a string.
   //!
   //! <b>Postcondition</b>: x == *this.
   //! 
   //! <b>Complexity</b>: Linear to the elements x contains.
   basic_string& operator=(BOOST_COPY_ASSIGN_REF(basic_string) x)
   {
      if (&x != this){
         allocator_type &this_alloc     = this->alloc();
         const allocator_type &x_alloc  = x.alloc();
         container_detail::bool_<allocator_traits_type::
            propagate_on_container_copy_assignment::value> flag;
         if(flag && this_alloc != x_alloc){
            if(!this->is_short()){
               this->deallocate_block();
               this->is_short(true);
               Traits::assign(*this->priv_addr(), this->priv_null());
               this->priv_size(0);
            }
         }
         container_detail::assign_alloc(this->alloc(), x.alloc(), flag);
         this->assign(x.begin(), x.end());
      }
      return *this;
   }

   //! <b>Effects</b>: Move constructor. Moves mx's resources to *this.
   //!
   //! <b>Throws</b>: If allocator_type's copy constructor throws.
   //! 
   //! <b>Complexity</b>: Constant.
   basic_string& operator=(BOOST_RV_REF(basic_string) x)
   {
      if (&x != this){
         allocator_type &this_alloc = this->alloc();
         allocator_type &x_alloc    = x.alloc();
         //If allocators are equal we can just swap pointers
         if(this_alloc == x_alloc){
            //Destroy objects but retain memory in case x reuses it in the future
            this->clear();
            this->swap_data(x);
            //Move allocator if needed
            container_detail::bool_<allocator_traits_type::
               propagate_on_container_move_assignment::value> flag;
            container_detail::move_alloc(this_alloc, x_alloc, flag);
         }
         //If unequal allocators, then do a one by one move
         else{
            this->assign( x.begin(), x.end());
         }
      }
      return *this;
   }

   //! <b>Effects</b>: Assignment from a null-terminated c-string.
   basic_string& operator=(const CharT* s) 
   { return this->assign(s, s + Traits::length(s)); }

   //! <b>Effects</b>: Assignment from character.
   basic_string& operator=(CharT c)
   { return this->assign(static_cast<size_type>(1), c); }

   //! <b>Effects</b>: Returns an iterator to the first element contained in the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator begin()
   { return this->priv_addr(); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const
   { return this->priv_addr(); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cbegin() const
   { return this->priv_addr(); }

   //! <b>Effects</b>: Returns an iterator to the end of the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator end()
   { return this->priv_addr() + this->priv_size(); }

   //! <b>Effects</b>: Returns a const_iterator to the end of the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator end() const 
   { return this->priv_addr() + this->priv_size(); }  

   //! <b>Effects</b>: Returns a const_iterator to the end of the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cend() const 
   { return this->priv_addr() + this->priv_size(); }  

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning 
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rbegin()             
   { return reverse_iterator(this->priv_addr() + this->priv_size()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning 
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rbegin() const 
   { return this->crbegin(); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning 
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crbegin() const 
   { return const_reverse_iterator(this->priv_addr() + this->priv_size()); }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rend()               
   { return reverse_iterator(this->priv_addr()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rend()   const 
   { return this->crend(); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed vector. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crend()   const 
   { return const_reverse_iterator(this->priv_addr()); }

   //! <b>Effects</b>: Returns a copy of the internal allocator.
   //! 
   //! <b>Throws</b>: If allocator's copy constructor throws.
   //! 
   //! <b>Complexity</b>: Constant.
   allocator_type get_allocator() const 
   { return this->alloc(); }

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //! 
   //! <b>Throws</b>: Nothing
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Non-standard extension.
   const stored_allocator_type &get_stored_allocator() const BOOST_CONTAINER_NOEXCEPT
   {  return this->alloc(); }

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //! 
   //! <b>Throws</b>: Nothing
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Non-standard extension.
   stored_allocator_type &get_stored_allocator() BOOST_CONTAINER_NOEXCEPT
   {  return this->alloc(); }

   //! <b>Effects</b>: Returns the number of the elements contained in the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   size_type size() const    
   { return this->priv_size(); }

   //! <b>Effects</b>: Returns the number of the elements contained in the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   size_type length() const
   { return this->size(); }

   //! <b>Effects</b>: Returns the largest possible size of the vector.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   size_type max_size() const
   { return base_t::max_size(); }

   //! <b>Effects</b>: Inserts or erases elements at the end such that
   //!   the size becomes n. New elements are copy constructed from x.
   //!
   //! <b>Throws</b>: If memory allocation throws
   //!
   //! <b>Complexity</b>: Linear to the difference between size() and new_size.
   void resize(size_type n, CharT c)
   {
      if (n <= size())
         this->erase(this->begin() + n, this->end());
      else
         this->append(n - this->size(), c);
   }

   //! <b>Effects</b>: Inserts or erases elements at the end such that
   //!   the size becomes n. New elements are default constructed.
   //!
   //! <b>Throws</b>: If memory allocation throws
   //!
   //! <b>Complexity</b>: Linear to the difference between size() and new_size.
   void resize(size_type n)
   { resize(n, this->priv_null()); }

   //! <b>Effects</b>: If n is less than or equal to capacity(), this call has no
   //!   effect. Otherwise, it is a request for allocation of additional memory.
   //!   If the request is successful, then capacity() is greater than or equal to
   //!   n; otherwise, capacity() is unchanged. In either case, size() is unchanged.
   //! 
   //! <b>Throws</b>: If memory allocation allocation throws
   void reserve(size_type res_arg)
   {
      if (res_arg > this->max_size())
         this->throw_length_error();

      if (this->capacity() < res_arg){
         size_type n = container_detail::max_value(res_arg, this->size()) + 1;
         size_type new_cap = this->next_capacity(n);
         pointer new_start = this->allocation_command
            (allocate_new, n, new_cap, new_cap).first;
         size_type new_length = 0;

         new_length += priv_uninitialized_copy
            (this->priv_addr(), this->priv_addr() + this->priv_size(), new_start);
         this->priv_construct_null(new_start + new_length);
         this->deallocate_block();
         this->is_short(false);
         this->priv_long_addr(new_start);
         this->priv_size(new_length);
         this->priv_storage(new_cap);
      }
   }

   //! <b>Effects</b>: Number of elements for which memory has been allocated.
   //!   capacity() is always greater than or equal to size().
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   size_type capacity() const
   { return this->priv_capacity(); }

   //! <b>Effects</b>: Erases all the elements of the vector.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the vector.
   void clear()
   {
      if (!empty()) {
         Traits::assign(*this->priv_addr(), this->priv_null());
         this->priv_size(0);
      }
   }

   //! <b>Effects</b>: Tries to deallocate the excess of memory created
   //!   with previous allocations. The size of the string is unchanged
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Complexity</b>: Linear to size().
   void shrink_to_fit()
   {
      //Check if shrinking is possible
      if(this->priv_storage() > InternalBufferChars){
         //Check if we should pass from dynamically allocated buffer
         //to the internal storage
         if(this->priv_size() < (InternalBufferChars)){
            //Dynamically allocated buffer attributes
            pointer   long_addr    = this->priv_long_addr();
            size_type long_storage = this->priv_long_storage();
            size_type long_size    = this->priv_long_size();
            //Shrink from allocated buffer to the internal one, including trailing null
            Traits::copy( container_detail::to_raw_pointer(this->priv_short_addr())
                        , container_detail::to_raw_pointer(long_addr)
                        , long_size+1);
            this->is_short(true);
            this->alloc().deallocate(long_addr, long_storage);
         }
         else{
            //Shrinking in dynamic buffer
            this->priv_shrink_to_fit_dynamic_buffer(alloc_version());
         }
      }
   }

   //! <b>Effects</b>: Returns true if the vector contains no elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   bool empty() const
   { return !this->priv_size(); }

   //! <b>Requires</b>: size() > n.
   //!
   //! <b>Effects</b>: Returns a reference to the nth element 
   //!   from the beginning of the container.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference operator[](size_type n)
      { return *(this->priv_addr() + n); }

   //! <b>Requires</b>: size() > n.
   //!
   //! <b>Effects</b>: Returns a const reference to the nth element 
   //!   from the beginning of the container.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference operator[](size_type n) const
      { return *(this->priv_addr() + n); }

   //! <b>Requires</b>: size() > n.
   //!
   //! <b>Effects</b>: Returns a reference to the nth element 
   //!   from the beginning of the container.
   //! 
   //! <b>Throws</b>: std::range_error if n >= size()
   //! 
   //! <b>Complexity</b>: Constant.
   reference at(size_type n) {
      if (n >= size())
      this->throw_out_of_range();
      return *(this->priv_addr() + n);
   }

   //! <b>Requires</b>: size() > n.
   //!
   //! <b>Effects</b>: Returns a const reference to the nth element 
   //!   from the beginning of the container.
   //! 
   //! <b>Throws</b>: std::range_error if n >= size()
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference at(size_type n) const {
      if (n >= size())
         this->throw_out_of_range();
      return *(this->priv_addr() + n);
   }

   //! <b>Effects</b>: Calls append(str.data, str.size()).
   //!
   //! <b>Returns</b>: *this
   basic_string& operator+=(const basic_string& s)
   {  return this->append(s); }

   //! <b>Effects</b>: Calls append(s).
   //!
   //! <b>Returns</b>: *this
   basic_string& operator+=(const CharT* s)
   {  return this->append(s); }

   //! <b>Effects</b>: Calls append(1, c).
   //!
   //! <b>Returns</b>: *this
   basic_string& operator+=(CharT c)
   {  this->push_back(c); return *this;   }

   //! <b>Effects</b>: Calls append(str.data(), str.size()).
   //!
   //! <b>Returns</b>: *this
   basic_string& append(const basic_string& s) 
   {  return this->append(s.begin(), s.end());  }

   //! <b>Requires</b>: pos <= str.size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to append 
   //! as the smaller of n and str.size() - pos and calls append(str.data() + pos, rlen).
   //!
   //! <b>Throws</b>: If memory allocation throws and out_of_range if pos > str.size()
   //!
   //! <b>Returns</b>: *this
   basic_string& append(const basic_string& s, size_type pos, size_type n)
   {
      if (pos > s.size())
      this->throw_out_of_range();
      return this->append(s.begin() + pos,
                          s.begin() + pos + container_detail::min_value(n, s.size() - pos));
   }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Effects</b>: The function replaces the string controlled by *this with
   //!   a string of length size() + n whose irst size() elements are a copy of the
   //!   original string controlled by *this and whose remaining
   //!   elements are a copy of the initial n elements of s.
   //!
   //! <b>Throws</b>: If memory allocation throws length_error if size() + n > max_size().
   //!
   //! <b>Returns</b>: *this
   basic_string& append(const CharT* s, size_type n) 
   {  return this->append(s, s + n);  }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Effects</b>: Calls append(s, traits::length(s)).
   //!
   //! <b>Returns</b>: *this
   basic_string& append(const CharT* s) 
   {  return this->append(s, s + Traits::length(s));  }

   //! <b>Effects</b>: Equivalent to append(basic_string(n, c)).
   //!
   //! <b>Returns</b>: *this
   basic_string& append(size_type n, CharT c)
   {  return this->append(cvalue_iterator(c, n), cvalue_iterator()); }

   //! <b>Requires</b>: [first,last) is a valid range.
   //!
   //! <b>Effects</b>: Equivalent to append(basic_string(first, last)).
   //!
   //! <b>Returns</b>: *this
   template <class InputIter>
   basic_string& append(InputIter first, InputIter last)
   {  this->insert(this->end(), first, last);   return *this;  }

   //! <b>Effects</b>: Equivalent to append(static_cast<size_type>(1), c).
   void push_back(CharT c)
   {
      if (this->priv_size() < this->capacity()){
         this->priv_construct_null(this->priv_addr() + (this->priv_size() + 1));
         Traits::assign(this->priv_addr()[this->priv_size()], c);
         this->priv_size(this->priv_size()+1);
      }
      else{
         //No enough memory, insert a new object at the end
         this->append((size_type)1, c);
      }
   }

   //! <b>Effects</b>: Equivalent to assign(str, 0, npos).
   //!
   //! <b>Returns</b>: *this
   basic_string& assign(const basic_string& s) 
   {  return this->operator=(s); }

   //! <b>Effects</b>: The function replaces the string controlled by *this
   //!    with a string of length str.size() whose elements are a copy of the string 
   //!   controlled by str. Leaves str in a valid but unspecified state.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: *this
   basic_string& assign(BOOST_RV_REF(basic_string) ms) 
   {  return this->swap_data(ms), *this;  }

   //! <b>Requires</b>: pos <= str.size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to assign as
   //!   the smaller of n and str.size() - pos and calls assign(str.data() + pos rlen).
   //!
   //! <b>Throws</b>: If memory allocation throws or out_of_range if pos > str.size().
   //!
   //! <b>Returns</b>: *this
   basic_string& assign(const basic_string& s, 
                        size_type pos, size_type n) {
      if (pos > s.size())
      this->throw_out_of_range();
      return this->assign(s.begin() + pos, 
                          s.begin() + pos + container_detail::min_value(n, s.size() - pos));
   }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Effects</b>: Replaces the string controlled by *this with a string of
   //! length n whose elements are a copy of those pointed to by s.
   //!
   //! <b>Throws</b>: If memory allocation throws or length_error if n > max_size().
   //!    
   //! <b>Returns</b>: *this
   basic_string& assign(const CharT* s, size_type n)
   {  return this->assign(s, s + n);   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Effects</b>: Calls assign(s, traits::length(s)).
   //!
   //! <b>Returns</b>: *this
   basic_string& assign(const CharT* s)
   { return this->assign(s, s + Traits::length(s)); }

   //! <b>Effects</b>: Equivalent to assign(basic_string(n, c)).
   //!
   //! <b>Returns</b>: *this
   basic_string& assign(size_type n, CharT c)
   {  return this->assign(cvalue_iterator(c, n), cvalue_iterator()); }

   //! <b>Effects</b>: Equivalent to assign(basic_string(first, last)).
   //!
   //! <b>Returns</b>: *this
   template <class InputIter>
   basic_string& assign(InputIter first, InputIter last) 
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = container_detail::is_convertible<InputIter, size_type>::value;
      typedef container_detail::bool_<aux_boolean> Result;
      return this->priv_assign_dispatch(first, last, Result());
   }

   //! <b>Requires</b>: pos <= size().
   //!
   //! <b>Effects</b>: Calls insert(pos, str.data(), str.size()).
   //!
   //! <b>Throws</b>: If memory allocation throws or out_of_range if pos > size().
   //!
   //! <b>Returns</b>: *this
   basic_string& insert(size_type pos, const basic_string& s) 
   {
      if (pos > size())
         this->throw_out_of_range();
      if (this->size() > this->max_size() - s.size())
         this->throw_length_error();
      this->insert(this->priv_addr() + pos, s.begin(), s.end());
      return *this;
   }

   //! <b>Requires</b>: pos1 <= size() and pos2 <= str.size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to insert as
   //!   the smaller of n and str.size() - pos2 and calls insert(pos1, str.data() + pos2, rlen).
   //!
   //! <b>Throws</b>: If memory allocation throws or out_of_range if pos1 > size() or pos2 > str.size().
   //!
   //! <b>Returns</b>: *this
   basic_string& insert(size_type pos1, const basic_string& s,
                        size_type pos2, size_type n) 
   {
      if (pos1 > this->size() || pos2 > s.size())
         this->throw_out_of_range();
      size_type len = container_detail::min_value(n, s.size() - pos2);
      if (this->size() > this->max_size() - len)
         this->throw_length_error();
      const CharT *beg_ptr = container_detail::to_raw_pointer(s.begin()) + pos2;
      const CharT *end_ptr = beg_ptr + len;
      this->insert(this->priv_addr() + pos1, beg_ptr, end_ptr);
      return *this;
   }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT and pos <= size().
   //!
   //! <b>Effects</b>: Replaces the string controlled by *this with a string of length size() + n
   //!   whose first pos elements are a copy of the initial elements of the original string
   //!   controlled by *this and whose next n elements are a copy of the elements in s and whose
   //!   remaining elements are a copy of the remaining elements of the original string controlled by *this.
   //!
   //! <b>Throws</b>: If memory allocation throws, out_of_range if pos > size() or
   //!   length_error if size() + n > max_size().
   //!
   //! <b>Returns</b>: *this
   basic_string& insert(size_type pos, const CharT* s, size_type n) 
   {
      if (pos > this->size())
         this->throw_out_of_range();
      if (this->size() > this->max_size() - n)
         this->throw_length_error();
      this->insert(this->priv_addr() + pos, s, s + n);
      return *this;
   }

   //! <b>Requires</b>: pos <= size() and s points to an array of at least traits::length(s) + 1 elements of CharT
   //!
   //! <b>Effects</b>: Calls insert(pos, s, traits::length(s)).
   //!
   //! <b>Throws</b>: If memory allocation throws, out_of_range if pos > size()
   //!   length_error if size() > max_size() - Traits::length(s)
   //!
   //! <b>Returns</b>: *this
   basic_string& insert(size_type pos, const CharT* s) 
   {
      if (pos > size())
         this->throw_out_of_range();
      size_type len = Traits::length(s);
      if (this->size() > this->max_size() - len)
         this->throw_length_error();
      this->insert(this->priv_addr() + pos, s, s + len);
      return *this;
   }

   //! <b>Effects</b>: Equivalent to insert(pos, basic_string(n, c)).
   //!
   //! <b>Throws</b>: If memory allocation throws, out_of_range if pos > size()
   //!   length_error if size() > max_size() - n
   //!
   //! <b>Returns</b>: *this
   basic_string& insert(size_type pos, size_type n, CharT c) 
   {
      if (pos > this->size())
         this->throw_out_of_range();
      if (this->size() > this->max_size() - n)
         this->throw_length_error();
      this->insert(const_iterator(this->priv_addr() + pos), n, c);
      return *this;
   }

   //! <b>Requires</b>: p is a valid iterator on *this.
   //!
   //! <b>Effects</b>: inserts a copy of c before the character referred to by p.
   //!
   //! <b>Returns</b>: An iterator which refers to the copy of the inserted character.
   iterator insert(const_iterator p, CharT c) 
   {
      size_type new_offset = p - this->priv_addr() + 1;
      this->insert(p, cvalue_iterator(c, 1), cvalue_iterator());
      return this->priv_addr() + new_offset;
   }


   //! <b>Requires</b>: p is a valid iterator on *this.
   //!
   //! <b>Effects</b>: Inserts n copies of c before the character referred to by p.
   //!
   //! <b>Returns</b>: An iterator which refers to the copy of the first
   //!   inserted character, or p if n == 0.
   void insert(const_iterator p, size_type n, CharT c)
   {
      this->insert(p, cvalue_iterator(c, n), cvalue_iterator());
   }

   //! <b>Requires</b>: p is a valid iterator on *this. [first,last) is a valid range.
   //!
   //! <b>Effects</b>: Equivalent to insert(p - begin(), basic_string(first, last)).
   //!
   //! <b>Returns</b>: An iterator which refers to the copy of the first
   //!   inserted character, or p if first == last.
   template <class InputIter>
   void insert(const_iterator p, InputIter first, InputIter last) 
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = container_detail::is_convertible<InputIter, size_type>::value;
      typedef container_detail::bool_<aux_boolean> Result;
      this->priv_insert_dispatch(p, first, last, Result());
   }

   //! <b>Requires</b>: pos <= size()
   //!
   //! <b>Effects</b>: Determines the effective length xlen of the string to be removed as the smaller of n and size() - pos.
   //!   The function then replaces the string controlled by *this with a string of length size() - xlen
   //!   whose first pos elements are a copy of the initial elements of the original string controlled by *this,
   //!   and whose remaining elements are a copy of the elements of the original string controlled by *this
   //!   beginning at position pos + xlen.
   //!
   //! <b>Throws</b>: out_of_range if pos > size().
   //!
   //! <b>Returns</b>: *this
   basic_string& erase(size_type pos = 0, size_type n = npos) 
   {
      if (pos > size())
         this->throw_out_of_range();
      erase(this->priv_addr() + pos, this->priv_addr() + pos + container_detail::min_value(n, size() - pos));
      return *this;
   }  

   //! <b>Effects</b>: Removes the character referred to by p.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: An iterator which points to the element immediately following p prior to the element being
   //!    erased. If no such element exists, end() is returned.
   iterator erase(const_iterator p) 
   {
      // The move includes the terminating null.
      CharT *ptr = const_cast<CharT*>(container_detail::to_raw_pointer(p));
      Traits::move(ptr,
                   container_detail::to_raw_pointer(p + 1), 
                   this->priv_size() - (p - this->priv_addr()));
      this->priv_size(this->priv_size()-1);
      return iterator(ptr);
   }

   //! <b>Requires</b>: first and last are valid iterators on *this, defining a range [first,last).
   //!
   //! <b>Effects</b>: Removes the characters in the range [first,last).
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: An iterator which points to the element pointed to by last prior to
   //!   the other elements being erased. If no such element exists, end() is returned.
   iterator erase(const_iterator first, const_iterator last)
   {
      CharT * f = const_cast<CharT*>(container_detail::to_raw_pointer(first));
      if (first != last) { // The move includes the terminating null.
         size_type num_erased = last - first;
         Traits::move(f,
                      container_detail::to_raw_pointer(last), 
                      (this->priv_size() + 1)-(last - this->priv_addr()));
         size_type new_length = this->priv_size() - num_erased;
         this->priv_size(new_length);
      }
      return iterator(f);
   }

   //! <b>Requires</b>: !empty()
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Effects</b>: Equivalent to erase(size() - 1, 1).
   void pop_back()
   {
      Traits::assign(this->priv_addr()[this->priv_size()-1], this->priv_null());
      this->priv_size(this->priv_size()-1);;
   }

   //! <b>Requires</b>: pos1 <= size().
   //!
   //! <b>Effects</b>: Calls replace(pos1, n1, str.data(), str.size()).
   //!
   //! <b>Throws</b>: if memory allocation throws or out_of_range if pos1 > size().
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(size_type pos1, size_type n1, const basic_string& str) 
   {
      if (pos1 > size())
         this->throw_out_of_range();
      const size_type len = container_detail::min_value(n1, size() - pos1);
      if (this->size() - len >= this->max_size() - str.size())
         this->throw_length_error();
      return this->replace(this->priv_addr() + pos1, this->priv_addr() + pos1 + len, 
                           str.begin(), str.end());
   }

   //! <b>Requires</b>: pos1 <= size() and pos2 <= str.size().
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to be
   //!   inserted as the smaller of n2 and str.size() - pos2 and calls
   //!   replace(pos1, n1, str.data() + pos2, rlen).
   //!
   //! <b>Throws</b>: if memory allocation throws, out_of_range if pos1 > size() or pos2 > str.size().
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(size_type pos1, size_type n1,
                         const basic_string& str, size_type pos2, size_type n2) 
   {
      if (pos1 > size() || pos2 > str.size())
         this->throw_out_of_range();
      const size_type len1 = container_detail::min_value(n1, size() - pos1);
      const size_type len2 = container_detail::min_value(n2, str.size() - pos2);
      if (this->size() - len1 >= this->max_size() - len2)
         this->throw_length_error();
      return this->replace(this->priv_addr() + pos1, this->priv_addr() + pos1 + len1,
                     str.priv_addr() + pos2, str.priv_addr() + pos2 + len2);
   }

   //! <b>Requires</b>: pos1 <= size() and s points to an array of at least n2 elements of CharT.
   //!
   //! <b>Effects</b>: Determines the effective length xlen of the string to be removed as the 
   //!   smaller of n1 and size() - pos1. If size() - xlen >= max_size() - n2 throws length_error. 
   //!   Otherwise, the function replaces the string controlled by *this with a string of 
   //!   length size() - xlen + n2 whose first pos1 elements are a copy of the initial elements 
   //!   of the original string controlled by *this, whose next n2 elements are a copy of the 
   //!   initial n2 elements of s, and whose remaining elements are a copy of the elements of 
   //!   the original string controlled by *this beginning at position pos + xlen.
   //!
   //! <b>Throws</b>: if memory allocation throws, out_of_range if pos1 > size() or length_error 
   //!   if the length of the  resulting string would exceed max_size()
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(size_type pos1, size_type n1,
                        const CharT* s, size_type n2) 
   {
      if (pos1 > size())
         this->throw_out_of_range();
      const size_type len = container_detail::min_value(n1, size() - pos1);
      if (n2 > this->max_size() || size() - len >= this->max_size() - n2)
         this->throw_length_error();
      return this->replace(this->priv_addr() + pos1, this->priv_addr() + pos1 + len,
                     s, s + n2);
   }

   //! <b>Requires</b>: pos1 <= size() and s points to an array of at least n2 elements of CharT.
   //!
   //! <b>Effects</b>: Determines the effective length xlen of the string to be removed as the smaller
   //! of n1 and size() - pos1. If size() - xlen >= max_size() - n2 throws length_error. Otherwise,
   //! the function replaces the string controlled by *this with a string of length size() - xlen + n2
   //! whose first pos1 elements are a copy of the initial elements of the original string controlled
   //! by *this, whose next n2 elements are a copy of the initial n2 elements of s, and whose
   //! remaining elements are a copy of the elements of the original string controlled by *this
   //! beginning at position pos + xlen.
   //!
   //! <b>Throws</b>: if memory allocation throws, out_of_range if pos1 > size() or length_error 
   //!   if the length of the resulting string would exceed max_size()
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(size_type pos, size_type n1, const CharT* s) 
   {
      if (pos > size())
         this->throw_out_of_range();
      const size_type len = container_detail::min_value(n1, size() - pos);
      const size_type n2 = Traits::length(s);
      if (n2 > this->max_size() || size() - len >= this->max_size() - n2)
         this->throw_length_error();
      return this->replace(this->priv_addr() + pos, this->priv_addr() + pos + len,
                     s, s + Traits::length(s));
   }

   //! <b>Requires</b>: pos1 <= size().
   //!
   //! <b>Effects</b>: Equivalent to replace(pos1, n1, basic_string(n2, c)).
   //!
   //! <b>Throws</b>: if memory allocation throws, out_of_range if pos1 > size() or length_error 
   //!   if the length of the  resulting string would exceed max_size()
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(size_type pos1, size_type n1, size_type n2, CharT c) 
   {
      if (pos1 > size())
         this->throw_out_of_range();
      const size_type len = container_detail::min_value(n1, size() - pos1);
      if (n2 > this->max_size() || size() - len >= this->max_size() - n2)
         this->throw_length_error();
      return this->replace(this->priv_addr() + pos1, this->priv_addr() + pos1 + len, n2, c);
   }

   //! <b>Requires</b>: [begin(),i1) and [i1,i2) are valid ranges.
   //!
   //! <b>Effects</b>: Calls replace(i1 - begin(), i2 - i1, str).
   //!
   //! <b>Throws</b>: if memory allocation throws
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(const_iterator i1, const_iterator i2, const basic_string& str) 
   { return this->replace(i1, i2, str.begin(), str.end()); }

   //! <b>Requires</b>: [begin(),i1) and [i1,i2) are valid ranges and 
   //!   s points to an array of at least n elements
   //!
   //! <b>Effects</b>: Calls replace(i1 - begin(), i2 - i1, s, n).
   //!
   //! <b>Throws</b>: if memory allocation throws
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(const_iterator i1, const_iterator i2, const CharT* s, size_type n) 
   { return this->replace(i1, i2, s, s + n); }

   //! <b>Requires</b>: [begin(),i1) and [i1,i2) are valid ranges and s points to an
   //!   array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Effects</b>: Calls replace(i1 - begin(), i2 - i1, s, traits::length(s)).
   //!
   //! <b>Throws</b>: if memory allocation throws
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(const_iterator i1, const_iterator i2, const CharT* s) 
   {  return this->replace(i1, i2, s, s + Traits::length(s));   }

   //! <b>Requires</b>: [begin(),i1) and [i1,i2) are valid ranges.
   //!
   //! <b>Effects</b>: Calls replace(i1 - begin(), i2 - i1, basic_string(n, c)).
   //!
   //! <b>Throws</b>: if memory allocation throws
   //!
   //! <b>Returns</b>: *this
   basic_string& replace(const_iterator i1, const_iterator i2, size_type n, CharT c)
   {
      const size_type len = static_cast<size_type>(i2 - i1);
      if (len >= n) {
         Traits::assign(const_cast<CharT*>(container_detail::to_raw_pointer(i1)), n, c);
         erase(i1 + n, i2);
      }
      else {
         Traits::assign(const_cast<CharT*>(container_detail::to_raw_pointer(i1)), len, c);
         insert(i2, n - len, c);
      }
      return *this;
   }

   //! <b>Requires</b>: [begin(),i1), [i1,i2) and [j1,j2) are valid ranges.
   //!
   //! <b>Effects</b>: Calls replace(i1 - begin(), i2 - i1, basic_string(j1, j2)).
   //!
   //! <b>Throws</b>: if memory allocation throws
   //!
   //! <b>Returns</b>: *this
   template <class InputIter>
   basic_string& replace(const_iterator i1, const_iterator i2, InputIter j1, InputIter j2) 
   {
      //Dispatch depending on integer/iterator
      const bool aux_boolean = container_detail::is_convertible<InputIter, size_type>::value;
      typedef container_detail::bool_<aux_boolean> Result;
      return this->priv_replace_dispatch(i1, i2, j1, j2,  Result());
   }

   //! <b>Requires</b>: pos <= size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to copy as the
   //!   smaller of n and size() - pos. s shall designate an array of at least rlen elements.
   //!   The function then replaces the string designated by s with a string of length rlen 
   //!   whose elements are a copy of the string controlled by *this beginning at position pos.
   //!   The function does not append a null object to the string designated by s.
   //!
   //! <b>Throws</b>: if memory allocation throws, out_of_range if pos > size().
   //!
   //! <b>Returns</b>: rlen
   size_type copy(CharT* s, size_type n, size_type pos = 0) const 
   {
      if (pos > size())
         this->throw_out_of_range();
      const size_type len = container_detail::min_value(n, size() - pos);
      Traits::copy(s, container_detail::to_raw_pointer(this->priv_addr() + pos), len);
      return len;
   }

   //! <b>Effects</b>: *this contains the same sequence of characters that was in s, 
   //!   s contains the same sequence of characters that was in *this.
   //!
   //! <b>Throws</b>: Nothing
   void swap(basic_string& x)
   {
      this->base_t::swap_data(x);
      container_detail::bool_<allocator_traits_type::propagate_on_container_swap::value> flag;
      container_detail::swap_alloc(this->alloc(), x.alloc(), flag);
   }

   //! <b>Requires</b>: The program shall not alter any of the values stored in the character array.
   //!
   //! <b>Returns</b>: A pointer p such that p + i == &operator[](i) for each i in [0,size()].
   //!
   //! <b>Complexity</b>: constant time.
   const CharT* c_str() const 
   {  return container_detail::to_raw_pointer(this->priv_addr()); }

   //! <b>Requires</b>: The program shall not alter any of the values stored in the character array.
   //!
   //! <b>Returns</b>: A pointer p such that p + i == &operator[](i) for each i in [0,size()].
   //!
   //! <b>Complexity</b>: constant time.
   const CharT* data()  const 
   {  return container_detail::to_raw_pointer(this->priv_addr()); }

   //! <b>Effects</b>: Determines the lowest position xpos, if possible, such that both 
   //!   of the following conditions obtain: 19 pos <= xpos and xpos + str.size() <= size();
   //!   2) traits::eq(at(xpos+I), str.at(I)) for all elements I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type find(const basic_string& s, size_type pos = 0) const 
   { return find(s.c_str(), pos, s.size()); }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find(basic_string<CharT,traits,Allocator>(s,n),pos).
   size_type find(const CharT* s, size_type pos, size_type n) const
   {
      if (pos + n > size())
         return npos;
      else {
         pointer finish = this->priv_addr() + this->priv_size();
         const const_iterator result =
            std::search(container_detail::to_raw_pointer(this->priv_addr() + pos), 
                   container_detail::to_raw_pointer(finish),
                   s, s + n, Eq_traits<Traits>());
         return result != finish ? result - begin() : npos;
      }
   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find(basic_string(s), pos).
   size_type find(const CharT* s, size_type pos = 0) const 
   { return find(s, pos, Traits::length(s)); }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find(basic_string<CharT,traits,Allocator>(1,c), pos).
   size_type find(CharT c, size_type pos = 0) const
   {
      if (pos >= size())
         return npos;
      else {
         pointer finish = this->priv_addr() + this->priv_size();
         const const_iterator result =
            std::find_if(this->priv_addr() + pos, finish,
                  std::bind2nd(Eq_traits<Traits>(), c));
         return result != finish ? result - begin() : npos;
      }
   }

   //! <b>Effects</b>: Determines the highest position xpos, if possible, such 
   //!   that both of the following conditions obtain:
   //!   a) xpos <= pos and xpos + str.size() <= size();
   //!   b) traits::eq(at(xpos+I), str.at(I)) for all elements I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type rfind(const basic_string& str, size_type pos = npos) const 
      { return rfind(str.c_str(), pos, str.size()); }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: rfind(basic_string(s, n), pos).
   size_type rfind(const CharT* s, size_type pos, size_type n) const
   {
      const size_type len = size();

      if (n > len)
         return npos;
      else if (n == 0)
         return container_detail::min_value(len, pos);
      else {
         const const_iterator last = begin() + container_detail::min_value(len - n, pos) + n;
         const const_iterator result = find_end(begin(), last,
                                                s, s + n,
                                                Eq_traits<Traits>());
         return result != last ? result - begin() : npos;
      }
   }

   //! <b>Requires</b>: pos <= size() and s points to an array of at least 
   //!   traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: rfind(basic_string(s), pos).
   size_type rfind(const CharT* s, size_type pos = npos) const 
      { return rfind(s, pos, Traits::length(s)); }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: rfind(basic_string<CharT,traits,Allocator>(1,c),pos).
   size_type rfind(CharT c, size_type pos = npos) const
   {
      const size_type len = size();

      if (len < 1)
         return npos;
      else {
         const const_iterator last = begin() + container_detail::min_value(len - 1, pos) + 1;
         const_reverse_iterator rresult =
            std::find_if(const_reverse_iterator(last), rend(),
                  std::bind2nd(Eq_traits<Traits>(), c));
         return rresult != rend() ? (rresult.base() - 1) - begin() : npos;
      }
   }

   //! <b>Effects</b>: Determines the lowest position xpos, if possible, such that both of the
   //!   following conditions obtain: a) pos <= xpos and xpos < size();
   //!   b) traits::eq(at(xpos), str.at(I)) for some element I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type find_first_of(const basic_string& s, size_type pos = 0) const 
      { return find_first_of(s.c_str(), pos, s.size()); }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_of(basic_string(s, n), pos).
   size_type find_first_of(const CharT* s, size_type pos, size_type n) const
   {
      if (pos >= size())
         return npos;
      else {
         pointer finish = this->priv_addr() + this->priv_size();
         const_iterator result = std::find_first_of(this->priv_addr() + pos, finish,
                                                    s, s + n,
                                                    Eq_traits<Traits>());
         return result != finish ? result - begin() : npos;
      }
   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_of(basic_string(s), pos).
   size_type find_first_of(const CharT* s, size_type pos = 0) const 
      { return find_first_of(s, pos, Traits::length(s)); }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_of(basic_string<CharT,traits,Allocator>(1,c), pos).
   size_type find_first_of(CharT c, size_type pos = 0) const 
    { return find(c, pos); }

   //! <b>Effects</b>: Determines the highest position xpos, if possible, such that both of 
   //!   the following conditions obtain: a) xpos <= pos and xpos < size(); b)
   //!   traits::eq(at(xpos), str.at(I)) for some element I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type find_last_of(const basic_string& str, size_type pos = npos) const
      { return find_last_of(str.c_str(), pos, str.size()); }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_of(basic_string(s, n), pos).
   size_type find_last_of(const CharT* s, size_type pos, size_type n) const
   {
      const size_type len = size();

      if (len < 1)
         return npos;
      else {
         const const_iterator last = this->priv_addr() + container_detail::min_value(len - 1, pos) + 1;
         const const_reverse_iterator rresult =
            std::find_first_of(const_reverse_iterator(last), rend(),
                               s, s + n,
                               Eq_traits<Traits>());
         return rresult != rend() ? (rresult.base() - 1) - this->priv_addr() : npos;
      }
   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_of(basic_string<CharT,traits,Allocator>(1,c),pos).
   size_type find_last_of(const CharT* s, size_type pos = npos) const 
      { return find_last_of(s, pos, Traits::length(s)); }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_of(basic_string(s), pos).
   size_type find_last_of(CharT c, size_type pos = npos) const 
      {  return rfind(c, pos);   }

   //! <b>Effects</b>: Determines the lowest position xpos, if possible, such that 
   //!   both of the following conditions obtain:
   //!   a) pos <= xpos and xpos < size(); b) traits::eq(at(xpos), str.at(I)) for no
   //!   element I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type find_first_not_of(const basic_string& str, size_type pos = 0) const 
      { return find_first_not_of(str.c_str(), pos, str.size()); }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_not_of(basic_string(s, n), pos).
   size_type find_first_not_of(const CharT* s, size_type pos, size_type n) const
   {
      if (pos > size())
         return npos;
      else {
         pointer finish = this->priv_addr() + this->priv_size();
         const_iterator result = std::find_if(this->priv_addr() + pos, finish,
                                    Not_within_traits<Traits>(s, s + n));
         return result != finish ? result - this->priv_addr() : npos;
      }
   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_not_of(basic_string(s), pos).
   size_type find_first_not_of(const CharT* s, size_type pos = 0) const 
      { return find_first_not_of(s, pos, Traits::length(s)); }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_first_not_of(basic_string(1, c), pos).
   size_type find_first_not_of(CharT c, size_type pos = 0) const
   {
      if (pos > size())
         return npos;
      else {
         pointer finish = this->priv_addr() + this->priv_size();
         const_iterator result
            = std::find_if(this->priv_addr() + pos, finish,
                     std::not1(std::bind2nd(Eq_traits<Traits>(), c)));
         return result != finish ? result - begin() : npos;
      }
   }

   //! <b>Effects</b>: Determines the highest position xpos, if possible, such that
   //!   both of the following conditions obtain: a) xpos <= pos and xpos < size(); 
   //!   b) traits::eq(at(xpos), str.at(I)) for no element I of the string controlled by str.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: xpos if the function can determine such a value for xpos. Otherwise, returns npos.
   size_type find_last_not_of(const basic_string& str, size_type pos = npos) const
      { return find_last_not_of(str.c_str(), pos, str.size()); }

   //! <b>Requires</b>: s points to an array of at least n elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_not_of(basic_string(s, n), pos).
   size_type find_last_not_of(const CharT* s, size_type pos, size_type n) const
   {
      const size_type len = size();

      if (len < 1)
         return npos;
      else {
         const const_iterator last = begin() + container_detail::min_value(len - 1, pos) + 1;
         const const_reverse_iterator rresult =
            std::find_if(const_reverse_iterator(last), rend(),
                    Not_within_traits<Traits>(s, s + n));
         return rresult != rend() ? (rresult.base() - 1) - begin() : npos;
      }
   }

   //! <b>Requires</b>: s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_not_of(basic_string(s), pos).
   size_type find_last_not_of(const CharT* s, size_type pos = npos) const
      { return find_last_not_of(s, pos, Traits::length(s)); }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: find_last_not_of(basic_string(1, c), pos).
   size_type find_last_not_of(CharT c, size_type pos = npos) const
   {
      const size_type len = size();

      if (len < 1)
         return npos;
      else {
         const const_iterator last = begin() + container_detail::min_value(len - 1, pos) + 1;
         const_reverse_iterator rresult =
            std::find_if(const_reverse_iterator(last), rend(),
                  std::not1(std::bind2nd(Eq_traits<Traits>(), c)));
         return rresult != rend() ? (rresult.base() - 1) - begin() : npos;
      }
   }

   //! <b>Requires</b>: Requires: pos <= size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to copy as
   //!   the smaller of n and size() - pos.
   //!
   //! <b>Throws</b>: If memory allocation throws or out_of_range if pos > size().
   //!
   //! <b>Returns</b>: basic_string<CharT,traits,Allocator>(data()+pos,rlen).
   basic_string substr(size_type pos = 0, size_type n = npos) const 
   {
      if (pos > size())
         this->throw_out_of_range();
      return basic_string(this->priv_addr() + pos, 
                          this->priv_addr() + pos + container_detail::min_value(n, size() - pos), this->alloc());
   }

   //! <b>Effects</b>: Determines the effective length rlen of the string to copy as
   //!   the smaller of size() and str.size(). The function then compares the two strings by
   //!   calling traits::compare(data(), str.data(), rlen).
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: The nonzero result if the result of the comparison is nonzero.
   //!   Otherwise, returns a value < 0 if size() < str.size(), a 0 value if size() == str.size(),
   //!   and value > 0 if size() > str.size()
   int compare(const basic_string& str) const 
   { return s_compare(this->priv_addr(), this->priv_addr() + this->priv_size(), str.priv_addr(), str.priv_addr() + str.priv_size()); }

   //! <b>Requires</b>: pos1 <= size()
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to copy as
   //!   the smaller of
   //!
   //! <b>Throws</b>: out_of_range if pos1 > size()
   //!
   //! <b>Returns</b>:basic_string(*this,pos1,n1).compare(str).
   int compare(size_type pos1, size_type n1, const basic_string& str) const 
   {
      if (pos1 > size())
         this->throw_out_of_range();
      return s_compare(this->priv_addr() + pos1, 
                        this->priv_addr() + pos1 + container_detail::min_value(n1, size() - pos1),
                        str.priv_addr(), str.priv_addr() + str.priv_size());
   }

   //! <b>Requires</b>: pos1 <= size() and pos2 <= str.size() 
   //!
   //! <b>Effects</b>: Determines the effective length rlen of the string to copy as
   //!   the smaller of
   //!
   //! <b>Throws</b>: out_of_range if pos1 > size() or pos2 > str.size()
   //!
   //! <b>Returns</b>: basic_string(*this, pos1, n1).compare(basic_string(str, pos2, n2)).
   int compare(size_type pos1, size_type n1, 
               const basic_string& str, size_type pos2, size_type n2) const {
      if (pos1 > size() || pos2 > str.size())
         this->throw_out_of_range();
      return s_compare(this->priv_addr() + pos1, 
                        this->priv_addr() + pos1 + container_detail::min_value(n1, size() - pos1),
                        str.priv_addr() + pos2, 
                        str.priv_addr() + pos2 + container_detail::min_value(n2, size() - pos2));
   }

   //! <b>Throws</b>: Nothing
   //!
   //! <b>Returns</b>: compare(basic_string(s)).
   int compare(const CharT* s) const 
   {  return s_compare(this->priv_addr(), this->priv_addr() + this->priv_size(), s, s + Traits::length(s));   }


   //! <b>Requires</b>: pos1 > size() and s points to an array of at least n2 elements of CharT.
   //!
   //! <b>Throws</b>: out_of_range if pos1 > size()
   //!
   //! <b>Returns</b>: basic_string(*this, pos, n1).compare(basic_string(s, n2)).
   int compare(size_type pos1, size_type n1,
               const CharT* s, size_type n2) const 
   {
      if (pos1 > size())
         this->throw_out_of_range();
      return s_compare(this->priv_addr() + pos1, 
                        this->priv_addr() + pos1 + container_detail::min_value(n1, size() - pos1),
                        s, s + n2);
   }

   //! <b>Requires</b>: pos1 > size() and s points to an array of at least traits::length(s) + 1 elements of CharT.
   //!
   //! <b>Throws</b>: out_of_range if pos1 > size()
   //!
   //! <b>Returns</b>: basic_string(*this, pos, n1).compare(basic_string(s, n2)).
   int compare(size_type pos1, size_type n1, const CharT* s) const 
   {  return this->compare(pos1, n1, s, Traits::length(s)); }

   /// @cond
   private:
   static int s_compare(const_pointer f1, const_pointer l1,
                        const_pointer f2, const_pointer l2) 
   {
      const difference_type n1 = l1 - f1;
      const difference_type n2 = l2 - f2;
      const int cmp = Traits::compare(container_detail::to_raw_pointer(f1), 
                                      container_detail::to_raw_pointer(f2), 
                                      container_detail::min_value(n1, n2));
      return cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0));
   }

   template<class AllocVersion>
   void priv_shrink_to_fit_dynamic_buffer
      ( AllocVersion
      , typename container_detail::enable_if<container_detail::is_same<AllocVersion, allocator_v1> >::type* = 0)
   {
      //Allocate a new buffer.
      size_type real_cap = 0;
      pointer   long_addr    = this->priv_long_addr();
      size_type long_size    = this->priv_long_size();
      size_type long_storage = this->priv_long_storage();
      //We can make this nothrow as chars are always NoThrowCopyables
      try{
         std::pair<pointer, bool> ret = this->allocation_command
               (allocate_new, long_size+1, long_size+1, real_cap, long_addr);
         //Copy and update
         Traits::copy( container_detail::to_raw_pointer(ret.first)
                     , container_detail::to_raw_pointer(this->priv_long_addr())
                     , long_size+1);
         this->priv_long_addr(ret.first);
         this->priv_storage(real_cap);
         //And release old buffer
         this->alloc().deallocate(long_addr, long_storage);
      }
      catch(...){
         return;
      }
   }

   template<class AllocVersion>
   void priv_shrink_to_fit_dynamic_buffer
      ( AllocVersion 
      , typename container_detail::enable_if<container_detail::is_same<AllocVersion, allocator_v2> >::type* = 0)
   {
      size_type received_size;
      if(this->alloc().allocation_command
         ( shrink_in_place | nothrow_allocation
         , this->priv_long_storage(), this->priv_long_size()+1
         , received_size, this->priv_long_addr()).first){
         this->priv_storage(received_size);
      }
   }

   void priv_construct_null(pointer p)
   {  this->construct(p, 0);  }

   static CharT priv_null()
   {  return (CharT) 0; }

   // Helper functions used by constructors.  It is a severe error for
   // any of them to be called anywhere except from within constructors.
   void priv_terminate_string() 
   {  this->priv_construct_null(this->priv_addr() + this->priv_size());  }

   template <class InputIter>
   void priv_range_initialize(InputIter f, InputIter l,
                              std::input_iterator_tag)
   {
      this->allocate_initial_block(InternalBufferChars);
      this->priv_construct_null(this->priv_addr() + this->priv_size());
      this->append(f, l);
   }

   template <class ForwardIter>
   void priv_range_initialize(ForwardIter f, ForwardIter l, 
                              std::forward_iterator_tag)
   {
      difference_type n = std::distance(f, l);
      this->allocate_initial_block(container_detail::max_value<difference_type>(n+1, InternalBufferChars));
      priv_uninitialized_copy(f, l, this->priv_addr());
      this->priv_size(n);
      this->priv_terminate_string();
   }

   template <class InputIter>
   void priv_range_initialize(InputIter f, InputIter l)
   {
      typedef typename std::iterator_traits<InputIter>::iterator_category Category;
      this->priv_range_initialize(f, l, Category());
   }

   template <class Integer>
   void priv_initialize_dispatch(Integer n, Integer x, container_detail::true_)
   {
      this->allocate_initial_block(container_detail::max_value<difference_type>(n+1, InternalBufferChars));
      priv_uninitialized_fill_n(this->priv_addr(), n, x);
      this->priv_size(n);
      this->priv_terminate_string();
   }

   template <class InputIter>
   void priv_initialize_dispatch(InputIter f, InputIter l, container_detail::false_)
   {  this->priv_range_initialize(f, l);  }
 
   template<class FwdIt, class Count> inline
   void priv_uninitialized_fill_n(FwdIt first, Count count, const CharT val)
   {
      //Save initial position
      FwdIt init = first;

      BOOST_TRY{
         //Construct objects
         for (; count--; ++first){
            this->construct(first, val);
         }
      }
      BOOST_CATCH(...){
         //Call destructors
         for (; init != first; ++init){
            this->destroy(init);
         }
         BOOST_RETHROW
      }
      BOOST_CATCH_END
   }

   template<class InpIt, class FwdIt> inline
   size_type priv_uninitialized_copy(InpIt first, InpIt last, FwdIt dest)
   {
      //Save initial destination position
      FwdIt dest_init = dest;
      size_type constructed = 0;

      BOOST_TRY{
         //Try to build objects
         for (; first != last; ++dest, ++first, ++constructed){
            this->construct(dest, *first);
         }
      }
      BOOST_CATCH(...){
         //Call destructors
         for (; constructed--; ++dest_init){
            this->destroy(dest_init);
         }
         BOOST_RETHROW
      }
      BOOST_CATCH_END
      return (constructed);
   }

   template <class Integer>
   basic_string& priv_assign_dispatch(Integer n, Integer x, container_detail::true_) 
   {  return this->assign((size_type) n, (CharT) x);   }

   template <class InputIter>
   basic_string& priv_assign_dispatch(InputIter f, InputIter l,
                                      container_detail::false_)
   {
      size_type cur = 0;
      CharT *ptr = container_detail::to_raw_pointer(this->priv_addr());
      while (f != l && cur != this->priv_size()) {
         Traits::assign(*ptr, *f);
         ++f;
         ++cur;
         ++ptr;
      }
      if (f == l)
         this->erase(this->priv_addr() + cur, this->priv_addr() + this->priv_size());
      else
         this->append(f, l);
      return *this;
   }

   template <class InputIter>
   void priv_insert(const_iterator p, InputIter first, InputIter last, std::input_iterator_tag)
   {
      for ( ; first != last; ++first, ++p) {
         p = this->insert(p, *first);
      }
   }

   template <class ForwardIter>
   void priv_insert(const_iterator position, ForwardIter first, 
                    ForwardIter last,  std::forward_iterator_tag)
   {
      if (first != last) {
         size_type n = std::distance(first, last);
         size_type remaining = this->capacity() - this->priv_size();
         const size_type old_size = this->size();
         pointer old_start = this->priv_addr();
         bool enough_capacity = false;
         std::pair<pointer, bool> allocation_ret;
         size_type new_cap = 0;

         //Check if we have enough capacity
         if (remaining >= n){
            enough_capacity = true;            
         }
         else {
            //Otherwise expand current buffer or allocate new storage
            new_cap  = this->next_capacity(n);
            allocation_ret = this->allocation_command
                  (allocate_new | expand_fwd | expand_bwd, old_size + n + 1, 
                     new_cap, new_cap, old_start);

            //Check forward expansion
            if(old_start == allocation_ret.first){
               enough_capacity = true;
               this->priv_storage(new_cap);
            }
         }

         //Reuse same buffer
         if(enough_capacity){
            const size_type elems_after =
               this->priv_size() - (position - this->priv_addr());
            size_type old_length = this->priv_size();
            if (elems_after >= n) {
               pointer pointer_past_last = this->priv_addr() + this->priv_size() + 1;
               priv_uninitialized_copy(this->priv_addr() + (this->priv_size() - n + 1),
                                       pointer_past_last, pointer_past_last);

               this->priv_size(this->priv_size()+n);
               Traits::move(const_cast<CharT*>(container_detail::to_raw_pointer(position + n)),
                           container_detail::to_raw_pointer(position),
                           (elems_after - n) + 1);
               this->priv_copy(first, last, const_cast<CharT*>(container_detail::to_raw_pointer(position)));
            }
            else {
               ForwardIter mid = first;
               std::advance(mid, elems_after + 1);

               priv_uninitialized_copy(mid, last, this->priv_addr() + this->priv_size() + 1);
               this->priv_size(this->priv_size() + (n - elems_after));
               priv_uninitialized_copy
                  (position, const_iterator(this->priv_addr() + old_length + 1),
                  this->priv_addr() + this->priv_size());
               this->priv_size(this->priv_size() + elems_after);
               this->priv_copy(first, mid, const_cast<CharT*>(container_detail::to_raw_pointer(position)));
            }
         }
         else{
            pointer new_start = allocation_ret.first;
            if(!allocation_ret.second){
               //Copy data to new buffer
               size_type new_length = 0;
               //This can't throw, since characters are POD
               new_length += priv_uninitialized_copy
                              (const_iterator(this->priv_addr()), position, new_start);
               new_length += priv_uninitialized_copy
                              (first, last, new_start + new_length);
               new_length += priv_uninitialized_copy
                              (position, const_iterator(this->priv_addr() + this->priv_size()),
                              new_start + new_length);
               this->priv_construct_null(new_start + new_length);

               this->deallocate_block();
               this->is_short(false);
               this->priv_long_addr(new_start);
               this->priv_long_size(new_length);
               this->priv_long_storage(new_cap);
            }
            else{
               //value_type is POD, so backwards expansion is much easier 
               //than with vector<T>
               value_type *oldbuf = container_detail::to_raw_pointer(old_start);
               value_type *newbuf = container_detail::to_raw_pointer(new_start);
               const value_type *pos    = container_detail::to_raw_pointer(position);
               size_type  before  = pos - oldbuf;

               //First move old data
               Traits::move(newbuf, oldbuf, before);
               Traits::move(newbuf + before + n, pos, old_size - before);
               //Now initialize the new data
               priv_uninitialized_copy(first, last, new_start + before);
               this->priv_construct_null(new_start + (old_size + n));
               this->is_short(false);
               this->priv_long_addr(new_start);
               this->priv_long_size(old_size + n);
               this->priv_long_storage(new_cap);
            }
         }
      }
   }

   template <class Integer>
   void priv_insert_dispatch(const_iterator p, Integer n, Integer x,
                           container_detail::true_) 
   {  insert(p, (size_type) n, (CharT) x);   }

   template <class InputIter>
   void priv_insert_dispatch(const_iterator p, InputIter first, InputIter last,
                           container_detail::false_) 
   {
      typedef typename std::iterator_traits<InputIter>::iterator_category Category;
      priv_insert(p, first, last, Category());
   }

   template <class InputIterator, class OutIterator>
   void priv_copy(InputIterator first, InputIterator last, OutIterator result)
   {
      for ( ; first != last; ++first, ++result)
         Traits::assign(*result, *first);
   }

   void priv_copy(const CharT* first, const CharT* last, CharT* result) 
   {  Traits::copy(result, first, last - first);  }

   template <class Integer>
   basic_string& priv_replace_dispatch(const_iterator first, const_iterator last,
                                       Integer n, Integer x,
                                       container_detail::true_) 
   {  return this->replace(first, last, (size_type) n, (CharT) x);   }

   template <class InputIter>
   basic_string& priv_replace_dispatch(const_iterator first, const_iterator last,
                                       InputIter f, InputIter l,
                                       container_detail::false_) 
   {
      typedef typename std::iterator_traits<InputIter>::iterator_category Category;
      return this->priv_replace(first, last, f, l, Category());
   }


   template <class InputIter>
   basic_string& priv_replace(const_iterator first, const_iterator last,
                              InputIter f, InputIter l, std::input_iterator_tag)
   {
      for ( ; first != last && f != l; ++first, ++f)
         Traits::assign(*first, *f);

      if (f == l)
         this->erase(first, last);
      else
         this->insert(last, f, l);
      return *this;
   }

   template <class ForwardIter>
   basic_string& priv_replace(const_iterator first, const_iterator last,
                              ForwardIter f, ForwardIter l, 
                              std::forward_iterator_tag)
   {
      difference_type n = std::distance(f, l);
      const difference_type len = last - first;
      if (len >= n) {
         this->priv_copy(f, l, const_cast<CharT*>(container_detail::to_raw_pointer(first)));
         this->erase(first + n, last);
      }
      else {
         ForwardIter m = f;
         std::advance(m, len);
         this->priv_copy(f, m, const_cast<CharT*>(container_detail::to_raw_pointer(first)));
         this->insert(last, m, l);
      }
      return *this;
   }
   /// @endcond
};

//!Typedef for a basic_string of
//!narrow characters
typedef basic_string
   <char
   ,std::char_traits<char>
   ,std::allocator<char> >
string;

//!Typedef for a basic_string of
//!narrow characters
typedef basic_string
   <wchar_t
   ,std::char_traits<wchar_t>
   ,std::allocator<wchar_t> >
wstring;

/// @cond

template <class CharT, class Traits, class A> 
const typename basic_string<CharT,Traits,A>::size_type 
basic_string<CharT,Traits,A>::npos 
  = (typename basic_string<CharT,Traits,A>::size_type) -1;

/// @endcond

// ------------------------------------------------------------
// Non-member functions.

// Operator+

template <class CharT, class Traits, class A>
inline basic_string<CharT,Traits,A>
operator+(const basic_string<CharT,Traits,A>& x,
          const basic_string<CharT,Traits,A>& y)
{
   typedef basic_string<CharT,Traits,A> str_t;
   typedef typename str_t::reserve_t reserve_t;
   reserve_t reserve;
   str_t result(reserve, x.size() + y.size(), x.get_stored_allocator());
   result.append(x);
   result.append(y);
   return boost::move(result);
}

template <class CharT, class Traits, class A> inline
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
   operator+(
   BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) mx
   , BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) my)
{
   mx += my;
   return boost::move(mx);
}

template <class CharT, class Traits, class A> inline
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
   operator+(
   BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) mx
   , const basic_string<CharT,Traits,A>& y)
{
   mx += y;
   return boost::move(mx);
}

template <class CharT, class Traits, class A> inline
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
   operator+(const basic_string<CharT,Traits,A>& x,
         BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) my)
{
   typedef typename basic_string<CharT,Traits,A>::size_type size_type;
   my.replace(size_type(0), size_type(0), x);
   return boost::move(my);
}

template <class CharT, class Traits, class A>
inline basic_string<CharT,Traits,A>
operator+(const CharT* s, const basic_string<CharT,Traits,A>& y) 
{
   typedef basic_string<CharT, Traits, A> str_t;
   typedef typename str_t::reserve_t reserve_t;
   reserve_t reserve;
   const typename str_t::size_type n = Traits::length(s);
   str_t result(reserve, n + y.size());
   result.append(s, s + n);
   result.append(y);
   return boost::move(result);
}

template <class CharT, class Traits, class A> inline
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
operator+(const CharT* s,
         BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) my)
{
   typedef typename basic_string<CharT,Traits,A>::size_type size_type;
   return boost::move(my.replace(size_type(0), size_type(0), s));
}

template <class CharT, class Traits, class A>
inline basic_string<CharT,Traits,A>
operator+(CharT c, const basic_string<CharT,Traits,A>& y) 
{
   typedef basic_string<CharT,Traits,A> str_t;
   typedef typename str_t::reserve_t reserve_t;
   reserve_t reserve;
   str_t result(reserve, 1 + y.size());
   result.push_back(c);
   result.append(y);
   return boost::move(result);
}

template <class CharT, class Traits, class A> inline
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
operator+(CharT c,
         BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) my)
{
   typedef typename basic_string<CharT,Traits,A>::size_type size_type;
   return boost::move(my.replace(size_type(0), size_type(0), &c, &c + 1));
}

template <class CharT, class Traits, class A>
inline basic_string<CharT,Traits,A>
operator+(const basic_string<CharT,Traits,A>& x, const CharT* s) 
{
   typedef basic_string<CharT,Traits,A> str_t;
   typedef typename str_t::reserve_t reserve_t;
   reserve_t reserve;
   const typename str_t::size_type n = Traits::length(s);
   str_t result(reserve, x.size() + n, x.get_stored_allocator());
   result.append(x);
   result.append(s, s + n);
   return boost::move(result);
}

template <class CharT, class Traits, class A>
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
operator+(BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) mx
         , const CharT* s)
{
   mx += s;
   return boost::move(mx);
}

template <class CharT, class Traits, class A>
inline basic_string<CharT,Traits,A>
operator+(const basic_string<CharT,Traits,A>& x, const CharT c) 
{
   typedef basic_string<CharT,Traits,A> str_t;
   typedef typename str_t::reserve_t reserve_t;
   reserve_t reserve;
   str_t result(reserve, x.size() + 1, x.get_stored_allocator());
   result.append(x);
   result.push_back(c);
   return boost::move(result);
}

template <class CharT, class Traits, class A>
BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A)
operator+( BOOST_RV_REF_3_TEMPL_ARGS(basic_string, CharT, Traits, A) mx
         , const CharT c)
{
   mx += c;
   return boost::move(mx);
}

// Operator== and operator!=

template <class CharT, class Traits, class A>
inline bool
operator==(const basic_string<CharT,Traits,A>& x,
           const basic_string<CharT,Traits,A>& y) 
{
   return x.size() == y.size() &&
          Traits::compare(x.data(), y.data(), x.size()) == 0;
}

template <class CharT, class Traits, class A>
inline bool
operator==(const CharT* s, const basic_string<CharT,Traits,A>& y) 
{
   typename basic_string<CharT,Traits,A>::size_type n = Traits::length(s);
   return n == y.size() && Traits::compare(s, y.data(), n) == 0;
}

template <class CharT, class Traits, class A>
inline bool
operator==(const basic_string<CharT,Traits,A>& x, const CharT* s) 
{
   typename basic_string<CharT,Traits,A>::size_type n = Traits::length(s);
   return x.size() == n && Traits::compare(x.data(), s, n) == 0;
}

template <class CharT, class Traits, class A>
inline bool
operator!=(const basic_string<CharT,Traits,A>& x,
           const basic_string<CharT,Traits,A>& y) 
   {  return !(x == y);  }

template <class CharT, class Traits, class A>
inline bool
operator!=(const CharT* s, const basic_string<CharT,Traits,A>& y) 
   {  return !(s == y); }

template <class CharT, class Traits, class A>
inline bool
operator!=(const basic_string<CharT,Traits,A>& x, const CharT* s) 
   {  return !(x == s);   }


// Operator< (and also >, <=, and >=).

template <class CharT, class Traits, class A>
inline bool
operator<(const basic_string<CharT,Traits,A>& x, const basic_string<CharT,Traits,A>& y) 
{
   return x.compare(y) < 0;
//   return basic_string<CharT,Traits,A>
//      ::s_compare(x.begin(), x.end(), y.begin(), y.end()) < 0;
}

template <class CharT, class Traits, class A>
inline bool
operator<(const CharT* s, const basic_string<CharT,Traits,A>& y) 
{
   return y.compare(s) > 0;
//   basic_string<CharT,Traits,A>::size_type n = Traits::length(s);
//   return basic_string<CharT,Traits,A>
//          ::s_compare(s, s + n, y.begin(), y.end()) < 0;
}

template <class CharT, class Traits, class A>
inline bool
operator<(const basic_string<CharT,Traits,A>& x,
          const CharT* s) 
{
   return x.compare(s) < 0;
//   basic_string<CharT,Traits,A>::size_type n = Traits::length(s);
//   return basic_string<CharT,Traits,A>
//      ::s_compare(x.begin(), x.end(), s, s + n) < 0;
}

template <class CharT, class Traits, class A>
inline bool
operator>(const basic_string<CharT,Traits,A>& x,
          const basic_string<CharT,Traits,A>& y) {
   return y < x;
}

template <class CharT, class Traits, class A>
inline bool
operator>(const CharT* s, const basic_string<CharT,Traits,A>& y) {
   return y < s;
}

template <class CharT, class Traits, class A>
inline bool
operator>(const basic_string<CharT,Traits,A>& x, const CharT* s) 
{
   return s < x;
}

template <class CharT, class Traits, class A>
inline bool
operator<=(const basic_string<CharT,Traits,A>& x,
           const basic_string<CharT,Traits,A>& y) 
{
  return !(y < x);
}

template <class CharT, class Traits, class A>
inline bool
operator<=(const CharT* s, const basic_string<CharT,Traits,A>& y) 
   {  return !(y < s);  }

template <class CharT, class Traits, class A>
inline bool
operator<=(const basic_string<CharT,Traits,A>& x, const CharT* s) 
   {  return !(s < x);  }

template <class CharT, class Traits, class A>
inline bool
operator>=(const basic_string<CharT,Traits,A>& x,
           const basic_string<CharT,Traits,A>& y) 
   {  return !(x < y);  }

template <class CharT, class Traits, class A>
inline bool
operator>=(const CharT* s, const basic_string<CharT,Traits,A>& y) 
   {  return !(s < y);  }

template <class CharT, class Traits, class A>
inline bool
operator>=(const basic_string<CharT,Traits,A>& x, const CharT* s) 
   {  return !(x < s);  }

// Swap.
template <class CharT, class Traits, class A>
inline void swap(basic_string<CharT,Traits,A>& x, basic_string<CharT,Traits,A>& y) 
{  x.swap(y);  }

/// @cond
// I/O.  
namespace container_detail {

template <class CharT, class Traits>
inline bool
string_fill(std::basic_ostream<CharT, Traits>& os,
                  std::basic_streambuf<CharT, Traits>* buf,
                  std::size_t n)
{
   CharT f = os.fill();
   std::size_t i;
   bool ok = true;

   for (i = 0; i < n; i++)
      ok = ok && !Traits::eq_int_type(buf->sputc(f), Traits::eof());
   return ok;
}

}  //namespace container_detail {
/// @endcond

template <class CharT, class Traits, class A>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const basic_string<CharT,Traits,A>& s)
{
   typename std::basic_ostream<CharT, Traits>::sentry sentry(os);
   bool ok = false;

   if (sentry) {
      ok = true;
      typename basic_string<CharT,Traits,A>::size_type n = s.size();
      typename basic_string<CharT,Traits,A>::size_type pad_len = 0;
      const bool left = (os.flags() & std::ios::left) != 0;
      const std::size_t w = os.width(0);
      std::basic_streambuf<CharT, Traits>* buf = os.rdbuf();

      if (w != 0 && n < w)
         pad_len = w - n;
       
      if (!left)
         ok = container_detail::string_fill(os, buf, pad_len);    

      ok = ok && 
            buf->sputn(s.data(), std::streamsize(n)) == std::streamsize(n);

      if (left)
         ok = ok && container_detail::string_fill(os, buf, pad_len);
   }

   if (!ok)
      os.setstate(std::ios_base::failbit);

   return os;
}


template <class CharT, class Traits, class A>
std::basic_istream<CharT, Traits>& 
operator>>(std::basic_istream<CharT, Traits>& is, basic_string<CharT,Traits,A>& s)
{
   typename std::basic_istream<CharT, Traits>::sentry sentry(is);

   if (sentry) {
      std::basic_streambuf<CharT, Traits>* buf = is.rdbuf();
      const std::ctype<CharT>& ctype = std::use_facet<std::ctype<CharT> >(is.getloc());

      s.clear();
      std::size_t n = is.width(0);
      if (n == 0)
         n = static_cast<std::size_t>(-1);
      else
         s.reserve(n);

      while (n-- > 0) {
         typename Traits::int_type c1 = buf->sbumpc();

         if (Traits::eq_int_type(c1, Traits::eof())) {
            is.setstate(std::ios_base::eofbit);
            break;
         }
         else {
            CharT c = Traits::to_char_type(c1);

            if (ctype.is(std::ctype<CharT>::space, c)) {
               if (Traits::eq_int_type(buf->sputbackc(c), Traits::eof()))
                  is.setstate(std::ios_base::failbit);
               break;
            }
            else
               s.push_back(c);
         }
      }
      
      // If we have read no characters, then set failbit.
      if (s.size() == 0)
         is.setstate(std::ios_base::failbit);
   }
   else
      is.setstate(std::ios_base::failbit);

   return is;
}

template <class CharT, class Traits, class A>    
std::basic_istream<CharT, Traits>& 
getline(std::istream& is, basic_string<CharT,Traits,A>& s,CharT delim)
{
   typename basic_string<CharT,Traits,A>::size_type nread = 0;
   typename std::basic_istream<CharT, Traits>::sentry sentry(is, true);
   if (sentry) {
      std::basic_streambuf<CharT, Traits>* buf = is.rdbuf();
      s.clear();

      while (nread < s.max_size()) {
         int c1 = buf->sbumpc();
         if (Traits::eq_int_type(c1, Traits::eof())) {
            is.setstate(std::ios_base::eofbit);
            break;
         }
         else {
            ++nread;
            CharT c = Traits::to_char_type(c1);
            if (!Traits::eq(c, delim)) 
               s.push_back(c);
            else
               break;              // Character is extracted but not appended.
         }
      }
   }
   if (nread == 0 || nread >= s.max_size())
      is.setstate(std::ios_base::failbit);

   return is;
}

template <class CharT, class Traits, class A>    
inline std::basic_istream<CharT, Traits>& 
getline(std::basic_istream<CharT, Traits>& is, basic_string<CharT,Traits,A>& s)
{
   return getline(is, s, '\n');
}

template <class Ch, class A>
inline std::size_t hash_value(basic_string<Ch, std::char_traits<Ch>, A> const& v)
{
   return hash_range(v.begin(), v.end());
}

}}

/// @cond

namespace boost {
/*
//!has_trivial_destructor_after_move<> == true_type
//!specialization for optimizations
template <class C, class T, class A>
struct has_trivial_destructor_after_move<boost::container::basic_string<C, T, A> >
{
   static const bool value = has_trivial_destructor<A>::value;
};
*/
}

/// @endcond

#include "third_party/boost/boost/container/detail/config_end.hpp"

#endif // BOOST_CONTAINER_STRING_HPP
