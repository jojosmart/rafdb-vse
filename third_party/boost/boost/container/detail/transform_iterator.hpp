//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011.
// (C) Copyright Gennaro Prota 2003 - 2004.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONTAINER_DETAIL_TRANSFORM_ITERATORS_HPP
#define BOOST_CONTAINER_DETAIL_TRANSFORM_ITERATORS_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "config_begin.hpp"
#include "third_party/boost/boost/container/detail/workaround.hpp"
#include "third_party/boost/boost/container/detail/type_traits.hpp"
#include <iterator>

namespace boost {
namespace container { 

template <class PseudoReference>
struct operator_arrow_proxy
{
   operator_arrow_proxy(const PseudoReference &px)
      :  m_value(px)
   {}

   PseudoReference* operator->() const { return &m_value; }
   // This function is needed for MWCW and BCC, which won't call operator->
   // again automatically per 13.3.1.2 para 8
//   operator T*() const { return &m_value; }
   mutable PseudoReference m_value;
};

template <class T>
struct operator_arrow_proxy<T&>
{
   operator_arrow_proxy(T &px)
      :  m_value(px)
   {}

   T* operator->() const { return const_cast<T*>(&m_value); }
   // This function is needed for MWCW and BCC, which won't call operator->
   // again automatically per 13.3.1.2 para 8
//   operator T*() const { return &m_value; }
   T &m_value;
};

template <class Iterator, class UnaryFunction>
class transform_iterator
   : public UnaryFunction
   , public std::iterator
      < typename Iterator::iterator_category
      , typename container_detail::remove_reference<typename UnaryFunction::result_type>::type
      , typename Iterator::difference_type
      , operator_arrow_proxy<typename UnaryFunction::result_type>
      , typename UnaryFunction::result_type>
{
   public:
   explicit transform_iterator(const Iterator &it, const UnaryFunction &f = UnaryFunction())
      :  UnaryFunction(f), m_it(it)
   {}

   explicit transform_iterator()
      :  UnaryFunction(), m_it()
   {}

   //Constructors
   transform_iterator& operator++() 
   { increment();   return *this;   }

   transform_iterator operator++(int)
   {
      transform_iterator result (*this);
      increment();
      return result;
   }

   friend bool operator== (const transform_iterator& i, const transform_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i == i2); }

/*
   friend bool operator> (const transform_iterator& i, const transform_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i < i2); }
*/
   friend typename Iterator::difference_type operator- (const transform_iterator& i, const transform_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   transform_iterator& operator+=(typename Iterator::difference_type off)
   {  this->advance(off); return *this;   }

   transform_iterator operator+(typename Iterator::difference_type off) const
   {
      transform_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend transform_iterator operator+(typename Iterator::difference_type off, const transform_iterator& right)
   {  return right + off; }

   transform_iterator& operator-=(typename Iterator::difference_type off)
   {  this->advance(-off); return *this;   }

   transform_iterator operator-(typename Iterator::difference_type off) const
   {  return *this + (-off);  }

   typename UnaryFunction::result_type operator*() const
   { return dereference(); }

   operator_arrow_proxy<typename UnaryFunction::result_type>
      operator->() const
   { return operator_arrow_proxy<typename UnaryFunction::result_type>(dereference());  }

   Iterator & base()
   {  return m_it;   }

   const Iterator & base() const
   {  return m_it;   }

   private:
   Iterator m_it;

   void increment()
   { ++m_it; }

   void decrement()
   { --m_it; }

   bool equal(const transform_iterator &other) const
   {  return m_it == other.m_it;   }

   bool less(const transform_iterator &other) const
   {  return other.m_it < m_it;   }

   typename UnaryFunction::result_type dereference() const
   { return UnaryFunction::operator()(*m_it); }

   void advance(typename Iterator::difference_type n)
   {  std::advance(m_it, n); }

   typename Iterator::difference_type distance_to(const transform_iterator &other)const
   {  return std::distance(other.m_it, m_it); }
};

template <class Iterator, class UnaryFunc>
transform_iterator<Iterator, UnaryFunc>
make_transform_iterator(Iterator it, UnaryFunc fun)
{
   return transform_iterator<Iterator, UnaryFunc>(it, fun);
}

}  //namespace container { 
}  //namespace boost {

#include "third_party/boost/boost/container/detail/config_end.hpp"

#endif   //#ifndef BOOST_CONTAINER_DETAIL_TRANSFORM_ITERATORS_HPP
