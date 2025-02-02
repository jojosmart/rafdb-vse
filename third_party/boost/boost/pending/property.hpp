//  (C) Copyright Jeremy Siek 2004
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_HPP
#define BOOST_PROPERTY_HPP

#include "third_party/boost/boost/mpl/bool.hpp"

namespace boost {

  struct no_property {
    typedef no_property tag_type;
    typedef no_property next_type;
    typedef no_property value_type;
    enum { num = 0 };
    typedef void kind;
  };

  template <class Tag, class T, class Base = no_property>
  struct property : public Base {
    typedef Base next_type;
    typedef Tag tag_type;
    typedef T value_type;
#if BOOST_WORKAROUND (__GNUC__, < 3)
    property() { }
#else
    property() : m_value() { }
#endif
    property(const T& v) : m_value(v) { }
    property(const T& v, const Base& b) : Base(b), m_value(v) { }
    // copy constructor and assignment operator will be generated by compiler

    T m_value;
  };

  // The BGL properties specialize property_kind and
  // property_num, and use enum's for the Property type (see
  // graph/properties.hpp), but the user may want to use a class
  // instead with a nested kind type and num.  Also, we may want to
  // switch BGL back to using class types for properties at some point.

  template <class PropertyTag>
  struct property_kind {
    typedef typename PropertyTag::kind type;
  };

  template <class P>
  struct has_property : boost::mpl::true_ {};
  template <>
  struct has_property<no_property> : boost::mpl::false_ {};

} // namespace boost

#include "third_party/boost/boost/pending/detail/property.hpp"

namespace boost {

  template <class PropertyList, class Tag>
  struct property_value {
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    typedef typename detail::build_property_tag_value_alist<PropertyList>::type AList;
    typedef typename detail::extract_value<AList,Tag>::type type;
#else
    typedef typename detail::build_property_tag_value_alist<PropertyList>::type AList;
    typedef typename detail::ev_selector<AList>::type Extractor;
    typedef typename Extractor::template bind_<AList,Tag>::type type;
#endif
  };

  template <class Tag2>
  inline detail::error_property_not_found
  get_property_value(const no_property&, Tag2) {
    return detail::error_property_not_found();
  }

  template <class Tag1, class Tag2, class T1, class Base>
  inline typename property_value<property<Tag1,T1,Base>, Tag2>::type&
  get_property_value(property<Tag1,T1,Base>& p, Tag2 tag2) {
    BOOST_STATIC_CONSTANT(bool,
                          match = (detail::same_property<Tag1,Tag2>::value));
    typedef property<Tag1,T1,Base> Prop;
    typedef typename property_value<Prop, Tag2>::type T2;
    T2* t2 = 0;
    typedef detail::property_value_dispatch<match> Dispatcher;
    return Dispatcher::get_value(p, t2, tag2);
  }
  template <class Tag1, class Tag2, class T1, class Base>
  inline
  const typename property_value<property<Tag1,T1,Base>, Tag2>::type&
  get_property_value(const property<Tag1,T1,Base>& p, Tag2 tag2) {
    BOOST_STATIC_CONSTANT(bool,
                          match = (detail::same_property<Tag1,Tag2>::value));
    typedef property<Tag1,T1,Base> Prop;
    typedef typename property_value<Prop, Tag2>::type T2;
    T2* t2 = 0;
    typedef detail::property_value_dispatch<match> Dispatcher;
    return Dispatcher::const_get_value(p, t2, tag2);
  }

 namespace detail {

     /** This trait returns true if T is no_property. */
    template <typename T>
    struct is_no_property
        : mpl::bool_<is_same<T, no_property>::value>
    { };

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    /** @internal @name Retag Property List
     * This metafunction is used internally to normalize a property if it is
     * actually modeling a property. Specifically this is used in Boost.Graph
     * to map user-provided classes into bundled properties.
     */
    //@{
    // One base case of the recursive form (see below). This matches any
    // retag request that does not include a property<...> or no_property as
    // the FinalType. This is used for generating bundles in Boost.Graph.
    template<typename FinalTag, typename FinalType>
    struct retag_property_list
    {
        typedef property<FinalTag, FinalType> type;
        typedef FinalType retagged;
    };

    // Recursively retag the nested property list.
    template<typename FinalTag, typename Tag, typename T, typename Base>
    struct retag_property_list<FinalTag, property<Tag, T, Base> >
    {
    private:
        typedef retag_property_list<FinalTag, Base> next;

    public:
        typedef property<Tag, T, typename next::type> type;
        typedef typename next::retagged retagged;
    };

    // This base case will correctly deduce the final property type if the
    // retagged property is given in property form. This should not hide
    // the base case below.
    // NOTE: This addresses a problem of layering bundled properties in the BGL
    // where multiple retaggings will fail to deduce the correct retagged
    // type.
    template<typename FinalTag, typename FinalType>
    struct retag_property_list<FinalTag, property<FinalTag, FinalType> >
    {
    public:
        typedef property<FinalTag, FinalType> type;
        typedef FinalType retagged;
    };

    // A final base case of the retag_property_list, this will terminate a
    // properly structured list.
    template<typename FinalTag>
    struct retag_property_list<FinalTag, no_property>
    {
        typedef no_property type;
        typedef no_property retagged;
    };
    //@}
#endif
} // namespace detail

} // namesapce boost

#endif /* BOOST_PROPERTY_HPP */
