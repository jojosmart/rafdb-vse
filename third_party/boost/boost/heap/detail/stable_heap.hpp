// boost heap: helper classes for stable priority queues
//
// Copyright (C) 2010 Tim Blechmann
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HEAP_DETAIL_STABLE_HEAP_HPP
#define BOOST_HEAP_DETAIL_STABLE_HEAP_HPP

#include <limits>
#include <stdexcept>
#include <utility>

#include "third_party/boost/boost/cstdint.hpp"
#include "third_party/boost/boost/throw_exception.hpp"
#include "third_party/boost/boost/iterator/iterator_adaptor.hpp"

#include "third_party/boost/boost/heap/policies.hpp"
#include "third_party/boost/boost/heap/heap_merge.hpp"

namespace boost  {
namespace heap   {
namespace detail {


template<bool ConstantSize, class SizeType>
struct size_holder
{
    static const bool constant_time_size = ConstantSize;
    typedef SizeType  size_type;

    size_holder(void):
        size_(0)
    {}

#ifdef BOOST_HAS_RVALUE_REFS
    size_holder(size_holder && rhs):
        size_(rhs.size_)
    {
        rhs.size_ = 0;
    }

    size_holder(size_holder const & rhs):
        size_(rhs.size_)
    {}

    size_holder & operator=(size_holder && rhs)
    {
        size_ = rhs.size_;
        rhs.size_ = 0;
        return *this;
    }

    size_holder & operator=(size_holder const & rhs)
    {
        size_ = rhs.size_;
        return *this;
    }
#endif

    SizeType get_size() const
    {  return size_;  }

    void set_size(SizeType size)
    {  size_ = size; }

    void decrement()
    {  --size_; }

    void increment()
    {  ++size_; }

    void add(SizeType value)
    {  size_ += value; }

    void sub(SizeType value)
    {  size_ -= value; }

    void swap(size_holder & rhs)
    {  std::swap(size_, rhs.size_); }

    SizeType size_;
};

template<class SizeType>
struct size_holder<false, SizeType>
{
    static const bool constant_time_size = false;
    typedef SizeType  size_type;

    size_holder(void)
    {}

#ifdef BOOST_HAS_RVALUE_REFS
    size_holder(size_holder && rhs)
    {}

    size_holder(size_holder const & rhs)
    {}

    size_holder & operator=(size_holder && rhs)
    {
        return *this;
    }

    size_holder & operator=(size_holder const & rhs)
    {
        return *this;
    }
#endif

    size_type get_size() const
    {  return 0;  }

    void set_size(size_type)
    {}

    void decrement()
    {}

    void increment()
    {}

    void add(SizeType value)
    {}

    void sub(SizeType value)
    {}

    void swap(size_holder & rhs)
    {}
};

template <typename T,
          typename Cmp,
          bool constant_time_size,
          typename StabilityCounterType = boost::uintmax_t,
          bool stable = false
         >
struct heap_base:
    Cmp,
    size_holder<constant_time_size, size_t>
{
    typedef StabilityCounterType stability_counter_type;
    typedef T value_type;
    typedef T internal_type;
    typedef size_holder<constant_time_size, size_t> size_holder_type;
    typedef Cmp value_compare;
    typedef Cmp internal_compare;
    static const bool is_stable = stable;

    heap_base (Cmp const & cmp = Cmp()):
        Cmp(cmp)
    {}

#ifdef BOOST_HAS_RVALUE_REFS
    heap_base(heap_base && rhs):
        Cmp(std::move(static_cast<Cmp&>(rhs))),
        size_holder_type(std::move(static_cast<size_holder_type&>(rhs)))
    {}

    heap_base(heap_base const & rhs):
        Cmp(static_cast<Cmp const &>(rhs)),
        size_holder_type(static_cast<size_holder_type const &>(rhs))
    {}

    heap_base & operator=(heap_base && rhs)
    {
        Cmp::operator=(std::move(static_cast<Cmp&>(rhs)));
        size_holder_type::operator=(std::move(static_cast<size_holder_type&>(rhs)));
        return *this;
    }

    heap_base & operator=(heap_base const & rhs)
    {
        Cmp::operator=(static_cast<Cmp const &>(rhs));
        size_holder_type::operator=(static_cast<size_holder_type const &>(rhs));
        return *this;
    }
#endif

    bool operator()(internal_type const & lhs, internal_type const & rhs) const
    {
        return Cmp::operator()(lhs, rhs);
    }

    internal_type make_node(T const & val)
    {
        return val;
    }

#ifdef BOOST_HAS_RVALUE_REFS
    T && make_node(T && val)
    {
        return std::forward<T>(val);
    }
#endif

    static T & get_value(internal_type & val)
    {
        return val;
    }

    static T const & get_value(internal_type const & val)
    {
        return val;
    }

    Cmp const & value_comp(void) const
    {
        return *this;
    }

    Cmp const & get_internal_cmp(void) const
    {
        return *this;
    }

    void swap(heap_base & rhs)
    {
        std::swap(static_cast<Cmp&>(*this), static_cast<Cmp&>(rhs));
        size_holder<constant_time_size, size_t>::swap(rhs);
    }

    stability_counter_type get_stability_count(void) const
    {
        return 0;
    }

    void set_stability_count(stability_counter_type)
    {}

    template <typename Heap1, typename Heap2>
    friend struct heap_merge_emulate;
};

template <typename T,
          typename Cmp,
          bool constant_time_size,
          typename StabilityCounterType
         >
struct heap_base<T, Cmp, constant_time_size, StabilityCounterType, true>:
    Cmp,
    size_holder<constant_time_size, size_t>
{
    typedef StabilityCounterType stability_counter_type;
    typedef T value_type;
    typedef std::pair<T, stability_counter_type> internal_type;
    typedef size_holder<constant_time_size, size_t> size_holder_type;
    typedef Cmp value_compare;

    heap_base (Cmp const & cmp = Cmp()):
        Cmp(cmp), counter_(0)
    {}

#ifdef BOOST_HAS_RVALUE_REFS
    heap_base(heap_base && rhs):
        Cmp(std::move(static_cast<Cmp&>(rhs))),
        size_holder_type(std::move(static_cast<size_holder_type&>(rhs))), counter_(rhs.counter_)
    {
        rhs.counter_ = 0;
    }

    heap_base & operator=(heap_base && rhs)
    {
        Cmp::operator=(std::move(static_cast<Cmp&>(rhs)));
        size_holder_type::operator=(std::move(static_cast<size_holder_type&>(rhs)));

        counter_ = rhs.counter_;
        rhs.counter_ = 0;
        return *this;
    }

    heap_base & operator=(heap_base const & rhs)
    {
        Cmp::operator=(static_cast<Cmp const &>(rhs));
        size_holder_type::operator=(static_cast<size_holder_type const &>(rhs));

        counter_ = rhs.counter_;
        return *this;
    }

#endif

    bool operator()(internal_type const & lhs, internal_type const & rhs) const
    {
        internal_compare cmp(get_internal_cmp());
        return cmp(lhs, rhs);
    }

    bool operator()(T const & lhs, T const & rhs) const
    {
        return Cmp::operator()(lhs, rhs);
    }

    internal_type make_node(T const & val)
    {
        stability_counter_type count = ++counter_;
        if (counter_ == std::numeric_limits<stability_counter_type>::max())
            BOOST_THROW_EXCEPTION(std::runtime_error("boost::heap counter overflow"));
        return std::make_pair(val, count);
    }

#if defined(BOOST_HAS_RVALUE_REFS) && !defined(BOOST_NO_VARIADIC_TEMPLATES)
    template <class... Args>
    internal_type make_node(Args&&... args)
    {
        stability_counter_type count = ++counter_;
        if (counter_ == std::numeric_limits<stability_counter_type>::max())
            BOOST_THROW_EXCEPTION(std::runtime_error("boost::heap counter overflow"));
        return std::make_pair(std::forward<T>(args)..., count);
    }
#endif

    static T & get_value(internal_type & val)
    {
        return val.first;
    }

    static T const & get_value(internal_type const & val)
    {
        return val.first;
    }

    Cmp const & value_comp(void) const
    {
        return *this;
    }

    struct internal_compare:
        Cmp
    {
        internal_compare(Cmp const & cmp = Cmp()):
            Cmp(cmp)
        {}

        bool operator()(internal_type const & lhs, internal_type const & rhs) const
        {
            if (Cmp::operator()(lhs.first, rhs.first))
                return true;

            if (Cmp::operator()(rhs.first, lhs.first))
                return false;

            return lhs.second > rhs.second;
        }
    };

    internal_compare get_internal_cmp(void) const
    {
        return internal_compare(*this);
    }

    void swap(heap_base & rhs)
    {
        std::swap(static_cast<Cmp&>(*this), static_cast<Cmp&>(rhs));
        std::swap(counter_, rhs.counter_);
        size_holder<constant_time_size, size_t>::swap(rhs);
    }

    stability_counter_type get_stability_count(void) const
    {
        return counter_;
    }

    void set_stability_count(stability_counter_type new_count)
    {
        counter_ = new_count;
    }

    template <typename Heap1, typename Heap2>
    friend struct heap_merge_emulate;

private:
    stability_counter_type counter_;
};

template <typename node_pointer,
          typename extractor,
          typename reference
         >
struct node_handle
{
    explicit node_handle(node_pointer n = 0):
        node_(n)
    {}

    reference operator*() const
    {
        return extractor::get_value(node_->value);
    }

    node_pointer node_;
};

template <typename value_type,
          typename internal_type,
          typename extractor
         >
struct value_extractor
{
    value_type const & operator()(internal_type const & data) const
    {
        return extractor::get_value(data);
    }
};

template <typename T,
          typename ContainerIterator,
          typename Extractor>
class stable_heap_iterator:
    public boost::iterator_adaptor<stable_heap_iterator<T, ContainerIterator, Extractor>,
                                   ContainerIterator,
                                   T const,
                                   boost::random_access_traversal_tag>
{
    typedef boost::iterator_adaptor<stable_heap_iterator,
                                    ContainerIterator,
                                    T const,
                                    boost::random_access_traversal_tag> super_t;

public:
    stable_heap_iterator(void):
        super_t(0)
    {}

    explicit stable_heap_iterator(ContainerIterator const & it):
        super_t(it)
    {}

private:
    friend class boost::iterator_core_access;

    T const & dereference() const
    {
        return Extractor::get_value(*super_t::base());
    }
};

template <typename T, typename Parspec, bool constant_time_size>
struct make_heap_base
{
    typedef typename parameter::binding<Parspec, tag::compare, std::less<T> >::type compare_argument;
    typedef typename parameter::binding<Parspec, tag::allocator, std::allocator<T> >::type allocator_argument;
    typedef typename parameter::binding<Parspec, tag::stability_counter_type, boost::uintmax_t >::type stability_counter_type;

    static const bool is_stable = extract_stable<Parspec>::value;

    typedef heap_base<T, compare_argument, constant_time_size, stability_counter_type, is_stable> type;
};


template <typename Alloc>
struct extract_allocator_types
{
    typedef typename Alloc::size_type size_type;
    typedef typename Alloc::difference_type difference_type;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_reference const_reference;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_pointer const_pointer;
};


} /* namespace detail */
} /* namespace heap */
} /* namespace boost */

#endif /* BOOST_HEAP_DETAIL_STABLE_HEAP_HPP */
