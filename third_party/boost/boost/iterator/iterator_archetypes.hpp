// (C) Copyright Jeremy Siek 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ITERATOR_ARCHETYPES_HPP
#define BOOST_ITERATOR_ARCHETYPES_HPP

#include "third_party/boost/boost/iterator/iterator_categories.hpp"
#include "third_party/boost/boost/operators.hpp"
#include "third_party/boost/boost/static_assert.hpp"
#include "third_party/boost/boost/iterator.hpp"

#include "third_party/boost/boost/iterator/detail/facade_iterator_category.hpp"

#include "third_party/boost/boost/type_traits/is_const.hpp"
#include "third_party/boost/boost/type_traits/add_const.hpp"
#include "third_party/boost/boost/type_traits/remove_const.hpp"
#include "third_party/boost/boost/type_traits/remove_cv.hpp"

#include "third_party/boost/boost/concept_archetype.hpp"

#include "third_party/boost/boost/mpl/aux_/msvc_eti_base.hpp"
#include "third_party/boost/boost/mpl/bitand.hpp"
#include "third_party/boost/boost/mpl/int.hpp"
#include "third_party/boost/boost/mpl/equal_to.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/eval_if.hpp"
#include "third_party/boost/boost/mpl/and.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"

#include <cstddef>

namespace boost {

template <class Value, class AccessCategory>
struct access_archetype;

template <class Derived, class Value, class AccessCategory, class TraversalCategory>
struct traversal_archetype;

namespace iterator_archetypes
{
  enum {
      readable_iterator_bit = 1
    , writable_iterator_bit = 2
    , swappable_iterator_bit = 4
    , lvalue_iterator_bit = 8
  };

  // Not quite tags, since dispatching wouldn't work.
  typedef mpl::int_<readable_iterator_bit>::type readable_iterator_t;
  typedef mpl::int_<writable_iterator_bit>::type writable_iterator_t;
  
  typedef mpl::int_<
      (readable_iterator_bit|writable_iterator_bit)
          >::type readable_writable_iterator_t;
  
  typedef mpl::int_<
      (readable_iterator_bit|lvalue_iterator_bit)
          >::type readable_lvalue_iterator_t;
  
  typedef mpl::int_<
      (lvalue_iterator_bit|writable_iterator_bit)
          >::type writable_lvalue_iterator_t;
  
  typedef mpl::int_<swappable_iterator_bit>::type swappable_iterator_t;
  typedef mpl::int_<lvalue_iterator_bit>::type lvalue_iterator_t;

  template <class Derived, class Base>
  struct has_access
    : mpl::equal_to<
          mpl::bitand_<Derived,Base>
        , Base
      >
  {};
}

namespace detail
{
  template <class T>
  struct assign_proxy
  {
      assign_proxy& operator=(T) { return *this; }
  };

  template <class T>
  struct read_proxy
  {
      operator T() { return static_object<T>::get(); }
  };

  template <class T>
  struct read_write_proxy
    : read_proxy<T> // Use to inherit from assign_proxy, but that doesn't work. -JGS
  {
      read_write_proxy& operator=(T) { return *this; }
  };

  template <class T>
  struct arrow_proxy
  {
      T const* operator->() const { return 0; }
  };

  struct no_operator_brackets {};

  template <class ValueType>
  struct readable_operator_brackets
  {
      read_proxy<ValueType> operator[](std::ptrdiff_t n) const { return read_proxy<ValueType>(); }
  };

  template <class ValueType>
  struct writable_operator_brackets
  {
      read_write_proxy<ValueType> operator[](std::ptrdiff_t n) const { return read_write_proxy<ValueType>(); }
  };

  template <class Value, class AccessCategory, class TraversalCategory>
  struct operator_brackets
    : mpl::aux::msvc_eti_base<
          typename mpl::eval_if<
              is_convertible<TraversalCategory, random_access_traversal_tag>
            , mpl::eval_if<
                  iterator_archetypes::has_access<
                      AccessCategory
                    , iterator_archetypes::writable_iterator_t
                  >
                , mpl::identity<writable_operator_brackets<Value> >
                , mpl::if_<
                      iterator_archetypes::has_access<
                          AccessCategory
                        , iterator_archetypes::readable_iterator_t
                      >
                    , readable_operator_brackets<Value>
                    , no_operator_brackets
                  >
              >
            , mpl::identity<no_operator_brackets>
          >::type
      >::type
  {};
  
  template <class TraversalCategory>
  struct traversal_archetype_impl
  {
      template <class Derived,class Value> struct archetype;
  };

  // Constructor argument for those iterators that
  // are not default constructible
  struct ctor_arg {};

  template <class Derived, class Value, class TraversalCategory>
  struct traversal_archetype_
    : mpl::aux::msvc_eti_base<
          typename traversal_archetype_impl<TraversalCategory>::template archetype<Derived,Value>
      >::type
  {
      typedef typename
        traversal_archetype_impl<TraversalCategory>::template archetype<Derived,Value>
      base;
      
      traversal_archetype_() {}

      traversal_archetype_(ctor_arg arg)
        : base(arg) 
      {}
  };

  template <>
  struct traversal_archetype_impl<incrementable_traversal_tag>
  {
      template<class Derived, class Value>
      struct archetype
      {
          explicit archetype(ctor_arg) {}

          struct bogus { }; // This use to be void, but that causes trouble for iterator_facade. Need more research. -JGS
          typedef bogus difference_type;

          Derived& operator++() { return (Derived&)static_object<Derived>::get(); }
          Derived  operator++(int) const { return (Derived&)static_object<Derived>::get(); }
      };
  };

  template <>
  struct traversal_archetype_impl<single_pass_traversal_tag>
  {
      template<class Derived, class Value>
      struct archetype
        : public equality_comparable< traversal_archetype_<Derived, Value, single_pass_traversal_tag> >,
          public traversal_archetype_<Derived, Value, incrementable_traversal_tag>
      {
          explicit archetype(ctor_arg arg)
            : traversal_archetype_<Derived, Value, incrementable_traversal_tag>(arg)
          {}
          
          typedef std::ptrdiff_t difference_type;
      };
  };

  template <class Derived, class Value>
  bool operator==(traversal_archetype_<Derived, Value, single_pass_traversal_tag> const&,
                  traversal_archetype_<Derived, Value, single_pass_traversal_tag> const&) { return true; }
  
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
  // doesn't seem to pick up != from equality_comparable
  template <class Derived, class Value>
  bool operator!=(traversal_archetype_<Derived, Value, single_pass_traversal_tag> const&,
                  traversal_archetype_<Derived, Value, single_pass_traversal_tag> const&) { return true; }
#endif 
  template <>
  struct traversal_archetype_impl<forward_traversal_tag>
  {
      template<class Derived, class Value>
      struct archetype
        : public traversal_archetype_<Derived, Value, single_pass_traversal_tag>
      {
          archetype() 
            : traversal_archetype_<Derived, Value, single_pass_traversal_tag>(ctor_arg())
          {}
      };
  };

  template <>
  struct traversal_archetype_impl<bidirectional_traversal_tag>
  {
      template<class Derived, class Value>
      struct archetype
        : public traversal_archetype_<Derived, Value, forward_traversal_tag>
      {
          Derived& operator--() { return static_object<Derived>::get(); }
          Derived  operator--(int) const { return static_object<Derived>::get(); }
      };
  };

  template <>
  struct traversal_archetype_impl<random_access_traversal_tag>
  {
      template<class Derived, class Value>
      struct archetype
        : public traversal_archetype_<Derived, Value, bidirectional_traversal_tag> 
      {
          Derived& operator+=(std::ptrdiff_t) { return static_object<Derived>::get(); }
          Derived& operator-=(std::ptrdiff_t) { return static_object<Derived>::get(); }
      };
  };

  template <class Derived, class Value>
  Derived& operator+(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                     std::ptrdiff_t) { return static_object<Derived>::get(); }

  template <class Derived, class Value>
  Derived& operator+(std::ptrdiff_t,
                     traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return static_object<Derived>::get(); }

  template <class Derived, class Value>
  Derived& operator-(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                     std::ptrdiff_t)
      { return static_object<Derived>::get(); }

  template <class Derived, class Value>
  std::ptrdiff_t operator-(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                           traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return 0; }

  template <class Derived, class Value>
  bool operator<(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                 traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return true; }

  template <class Derived, class Value>
  bool operator>(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                 traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return true; }

  template <class Derived, class Value>
  bool operator<=(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                 traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return true; }

  template <class Derived, class Value>
  bool operator>=(traversal_archetype_<Derived, Value, random_access_traversal_tag> const&,
                 traversal_archetype_<Derived, Value, random_access_traversal_tag> const&)
      { return true; }

  struct bogus_type;

  template <class Value>
  struct convertible_type
    : mpl::if_< is_const<Value>,
                typename remove_const<Value>::type,
                bogus_type >
  {};

} // namespace detail


template <class> struct undefined;
  
template <class AccessCategory>
struct iterator_access_archetype_impl
{
    template <class Value> struct archetype;
};

template <class Value, class AccessCategory>
struct iterator_access_archetype
  : mpl::aux::msvc_eti_base<
        typename iterator_access_archetype_impl<
            AccessCategory
        >::template archetype<Value>
    >::type
{
};

template <>
struct iterator_access_archetype_impl<
    iterator_archetypes::readable_iterator_t
>
{
    template <class Value>
    struct archetype
    {
        typedef typename remove_cv<Value>::type value_type;
        typedef Value                           reference;
        typedef Value*                          pointer;

        value_type operator*() const { return static_object<value_type>::get(); }

        detail::arrow_proxy<Value> operator->() const { return detail::arrow_proxy<Value>(); }
    };
};

template <>
struct iterator_access_archetype_impl<
    iterator_archetypes::writable_iterator_t
>
{
    template <class Value>
    struct archetype
    {
# if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
        BOOST_STATIC_ASSERT(!is_const<Value>::value);
# endif 
        typedef void value_type;
        typedef void reference;
        typedef void pointer;

        detail::assign_proxy<Value> operator*() const { return detail::assign_proxy<Value>(); }
    };
};

template <>
struct iterator_access_archetype_impl<
    iterator_archetypes::readable_writable_iterator_t
>
{
    template <class Value>
    struct archetype
      : public virtual iterator_access_archetype<
            Value, iterator_archetypes::readable_iterator_t
        >
    {
        typedef detail::read_write_proxy<Value>    reference;

        detail::read_write_proxy<Value> operator*() const { return detail::read_write_proxy<Value>(); }
    };
};

template <>
struct iterator_access_archetype_impl<iterator_archetypes::readable_lvalue_iterator_t>
{
    template <class Value>
    struct archetype
      : public virtual iterator_access_archetype<
            Value, iterator_archetypes::readable_iterator_t
        >
    {
        typedef Value&    reference;

        Value& operator*() const { return static_object<Value>::get(); }
        Value* operator->() const { return 0; }
    };
};
  
template <>
struct iterator_access_archetype_impl<iterator_archetypes::writable_lvalue_iterator_t>
{
    template <class Value>
    struct archetype
      : public virtual iterator_access_archetype<
            Value, iterator_archetypes::readable_lvalue_iterator_t
        >
    {
# if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
        BOOST_STATIC_ASSERT((!is_const<Value>::value));
# endif 
    };
};
  

template <class Value, class AccessCategory, class TraversalCategory>
struct iterator_archetype;
  
template <class Value, class AccessCategory, class TraversalCategory>
struct traversal_archetype_base 
  : detail::operator_brackets<
        typename remove_cv<Value>::type
      , AccessCategory
      , TraversalCategory
    >
  , detail::traversal_archetype_<
        iterator_archetype<Value, AccessCategory, TraversalCategory>
      , Value
      , TraversalCategory
    >
{
};

namespace detail
{
  template <class Value, class AccessCategory, class TraversalCategory>
  struct iterator_archetype_base
    : iterator_access_archetype<Value, AccessCategory>
    , traversal_archetype_base<Value, AccessCategory, TraversalCategory>
  {
      typedef iterator_access_archetype<Value, AccessCategory> access;
      
      typedef typename detail::facade_iterator_category<
          TraversalCategory
        , typename mpl::eval_if<
              iterator_archetypes::has_access<
                  AccessCategory, iterator_archetypes::writable_iterator_t
              >
            , remove_const<Value>
            , add_const<Value>
          >::type
        , typename access::reference
      >::type iterator_category;

      // Needed for some broken libraries (see below)
      typedef boost::iterator<
          iterator_category
        , Value
        , typename traversal_archetype_base<
              Value, AccessCategory, TraversalCategory
          >::difference_type
        , typename access::pointer
        , typename access::reference
      > workaround_iterator_base;
  };
}

template <class Value, class AccessCategory, class TraversalCategory>
struct iterator_archetype
  : public detail::iterator_archetype_base<Value, AccessCategory, TraversalCategory>

    // These broken libraries require derivation from std::iterator
    // (or related magic) in order to handle iter_swap and other
    // iterator operations
# if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, < 310)           \
    || BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(0x20101))
  , public detail::iterator_archetype_base<
        Value, AccessCategory, TraversalCategory
    >::workaround_iterator_base
# endif 
{
    // Derivation from std::iterator above caused references to nested
    // types to be ambiguous, so now we have to redeclare them all
    // here.
# if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, < 310)           \
    || BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(0x20101))
    
    typedef detail::iterator_archetype_base<
        Value,AccessCategory,TraversalCategory
    > base;
    
    typedef typename base::value_type value_type;
    typedef typename base::reference reference;
    typedef typename base::pointer pointer;
    typedef typename base::difference_type difference_type;
    typedef typename base::iterator_category iterator_category;
# endif

    iterator_archetype() { }
    iterator_archetype(iterator_archetype const& x)
      : detail::iterator_archetype_base<
            Value
          , AccessCategory
          , TraversalCategory
        >(x)
    {}

    iterator_archetype& operator=(iterator_archetype const&)
        { return *this; }

# if 0
    // Optional conversion from mutable
    iterator_archetype(
        iterator_archetype<
        typename detail::convertible_type<Value>::type
      , AccessCategory
      , TraversalCategory> const&
    );
# endif
};

} // namespace boost


#endif // BOOST_ITERATOR_ARCHETYPES_HPP
