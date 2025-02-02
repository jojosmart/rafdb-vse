/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2009
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_LIST_HPP
#define BOOST_INTRUSIVE_LIST_HPP

#include "third_party/boost/boost/intrusive/detail/config_begin.hpp"
#include "third_party/boost/boost/intrusive/detail/assert.hpp"
#include "third_party/boost/boost/intrusive/intrusive_fwd.hpp"
#include "third_party/boost/boost/intrusive/list_hook.hpp"
#include "third_party/boost/boost/intrusive/circular_list_algorithms.hpp"
#include "third_party/boost/boost/intrusive/pointer_traits.hpp"
#include "third_party/boost/boost/intrusive/detail/clear_on_destructor_base.hpp"
#include "third_party/boost/boost/intrusive/detail/mpl.hpp"
#include "third_party/boost/boost/intrusive/link_mode.hpp"
#include "third_party/boost/boost/static_assert.hpp"
#include "third_party/boost/boost/intrusive/options.hpp"
#include "third_party/boost/boost/intrusive/pointer_traits.hpp"
#include "third_party/boost/boost/intrusive/detail/utilities.hpp"
#include <iterator>
#include <algorithm>
#include <functional>
#include <cstddef>
#include "third_party/boost/boost/move/move.hpp"

namespace boost {
namespace intrusive {

/// @cond

template <class ValueTraits, class SizeType, bool ConstantTimeSize>
struct listopt
{
   typedef ValueTraits  value_traits;
   typedef SizeType     size_type;
   static const bool constant_time_size = ConstantTimeSize;
};


template <class T>
struct list_defaults
   :  pack_options
      < none
      , base_hook<detail::default_list_hook>
      , constant_time_size<true>
      , size_type<std::size_t>
      >::type
{};

/// @endcond

//! The class template list is an intrusive container that mimics most of the 
//! interface of std::list as described in the C++ standard.
//!
//! The template parameter \c T is the type to be managed by the container.
//! The user can specify additional options and if no options are provided
//! default options are used.
//!
//! The container supports the following options:
//! \c base_hook<>/member_hook<>/value_traits<>,
//! \c constant_time_size<> and \c size_type<>.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
class list_impl
   :  private detail::clear_on_destructor_base< list_impl<Config> >
{
   template<class C> friend class detail::clear_on_destructor_base;
   //Public typedefs
   public:
   typedef typename Config::value_traits                             value_traits;
   /// @cond
   static const bool external_value_traits =
      detail::external_value_traits_is_true<value_traits>::value;
   typedef typename detail::eval_if_c
      < external_value_traits
      , detail::eval_value_traits<value_traits>
      , detail::identity<value_traits>
      >::type                                                        real_value_traits;
   /// @endcond
   typedef typename real_value_traits::pointer                       pointer;
   typedef typename real_value_traits::const_pointer                 const_pointer;
   typedef typename pointer_traits<pointer>::element_type            value_type;
   typedef typename pointer_traits<pointer>::reference               reference;
   typedef typename pointer_traits<const_pointer>::reference         const_reference;
   typedef typename pointer_traits<pointer>::difference_type         difference_type;
   typedef typename Config::size_type                                size_type;
   typedef list_iterator<list_impl, false>                           iterator;
   typedef list_iterator<list_impl, true>                            const_iterator;
   typedef boost::intrusive::detail::reverse_iterator<iterator>      reverse_iterator;
   typedef boost::intrusive::detail::reverse_iterator<const_iterator>const_reverse_iterator;
   typedef typename real_value_traits::node_traits                   node_traits;
   typedef typename node_traits::node                                node;
   typedef typename node_traits::node_ptr                            node_ptr;
   typedef typename node_traits::const_node_ptr                      const_node_ptr;
   typedef circular_list_algorithms<node_traits>                     node_algorithms;

   static const bool constant_time_size = Config::constant_time_size;
   static const bool stateful_value_traits = detail::is_stateful_value_traits<real_value_traits>::value;

   /// @cond

   private:
   typedef detail::size_holder<constant_time_size, size_type>          size_traits;

   //noncopyable
   BOOST_MOVABLE_BUT_NOT_COPYABLE(list_impl)

   enum { safemode_or_autounlink  = 
            (int)real_value_traits::link_mode == (int)auto_unlink   ||
            (int)real_value_traits::link_mode == (int)safe_link     };

   //Constant-time size is incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(constant_time_size && 
                        ((int)real_value_traits::link_mode == (int)auto_unlink)
                      ));

   //Const cast emulation for smart pointers
   static node_ptr uncast(const const_node_ptr & ptr)
   {  return pointer_traits<node_ptr>::const_cast_from(ptr);  }

   node_ptr get_root_node()
   {  return pointer_traits<node_ptr>::pointer_to(data_.root_plus_size_.root_);  }

   const_node_ptr get_root_node() const
   {  return pointer_traits<const_node_ptr>::pointer_to(data_.root_plus_size_.root_);  }

   struct root_plus_size : public size_traits
   {
      node root_;
   };

   struct data_t : public value_traits
   {
      typedef typename list_impl::value_traits value_traits;
      data_t(const value_traits &val_traits)
         :  value_traits(val_traits)
      {}

      root_plus_size root_plus_size_;
   } data_;

   size_traits &priv_size_traits()
   {  return data_.root_plus_size_;  }

   const size_traits &priv_size_traits() const
   {  return data_.root_plus_size_;  }

   const real_value_traits &get_real_value_traits(detail::bool_<false>) const
   {  return data_;  }

   const real_value_traits &get_real_value_traits(detail::bool_<true>) const
   {  return data_.get_value_traits(*this);  }

   real_value_traits &get_real_value_traits(detail::bool_<false>)
   {  return data_;  }

   real_value_traits &get_real_value_traits(detail::bool_<true>)
   {  return data_.get_value_traits(*this);  }

   const value_traits &priv_value_traits() const
   {  return data_;  }

   value_traits &priv_value_traits()
   {  return data_;  }

   protected:
   node &prot_root_node()
   {  return data_.root_plus_size_.root_; }

   node const &prot_root_node() const
   {  return data_.root_plus_size_.root_; }

   void prot_set_size(size_type s)
   {  data_.root_plus_size_.set_size(s);  }

   /// @endcond

   public:

   const real_value_traits &get_real_value_traits() const
   {  return this->get_real_value_traits(detail::bool_<external_value_traits>());  }

   real_value_traits &get_real_value_traits()
   {  return this->get_real_value_traits(detail::bool_<external_value_traits>());  }

   //! <b>Effects</b>: constructs an empty list. 
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: If real_value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks).
   list_impl(const value_traits &v_traits = value_traits())
      :  data_(v_traits)
   {  
      this->priv_size_traits().set_size(size_type(0));
      node_algorithms::init_header(this->get_root_node());  
   }

   //! <b>Requires</b>: Dereferencing iterator must yield an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Constructs a list equal to the range [first,last).
   //! 
   //! <b>Complexity</b>: Linear in std::distance(b, e). No copy constructors are called.  
   //! 
   //! <b>Throws</b>: If real_value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks).
   template<class Iterator>
   list_impl(Iterator b, Iterator e, const value_traits &v_traits = value_traits())
      :  data_(v_traits)
   {
      this->priv_size_traits().set_size(size_type(0));
      node_algorithms::init_header(this->get_root_node());
      this->insert(this->cend(), b, e);
   }

   //! <b>Effects</b>: to-do
   //!   
   list_impl(BOOST_RV_REF(list_impl) x)
      : data_(::boost::move(x.priv_value_traits()))
   {
      this->priv_size_traits().set_size(size_type(0));
      node_algorithms::init_header(this->get_root_node());  
      this->swap(x);
   }

   //! <b>Effects</b>: to-do
   //!   
   list_impl& operator=(BOOST_RV_REF(list_impl) x) 
   {  this->swap(x); return *this;  }

   //! <b>Effects</b>: If it's not a safe-mode or an auto-unlink value_type 
   //!   the destructor does nothing
   //!   (ie. no code is generated). Otherwise it detaches all elements from this. 
   //!   In this case the objects in the list are not deleted (i.e. no destructors 
   //!   are called), but the hooks according to the ValueTraits template parameter
   //!   are set to their default value.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the list, if 
   //!   it's a safe-mode or auto-unlink value . Otherwise constant. 
   ~list_impl() 
   {}

   //! <b>Requires</b>: value must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the back of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void push_back(reference value) 
   {
      node_ptr to_insert = get_real_value_traits().to_node_ptr(value);
      if(safemode_or_autounlink)
         BOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(node_algorithms::inited(to_insert));
      node_algorithms::link_before(this->get_root_node(), to_insert);
      this->priv_size_traits().increment();
   }

   //! <b>Requires</b>: value must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the front of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void push_front(reference value) 
   {
      node_ptr to_insert = get_real_value_traits().to_node_ptr(value);
      if(safemode_or_autounlink)
         BOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(node_algorithms::inited(to_insert));
      node_algorithms::link_before(node_traits::get_next(this->get_root_node()), to_insert); 
      this->priv_size_traits().increment();
   }

   //! <b>Effects</b>: Erases the last element of the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased element.
   void pop_back()
   {  return this->pop_back_and_dispose(detail::null_disposer());   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the last element of the list.
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Disposer>
   void pop_back_and_dispose(Disposer disposer)
   {
      node_ptr to_erase = node_traits::get_previous(this->get_root_node());
      node_algorithms::unlink(to_erase);
      this->priv_size_traits().decrement();
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(get_real_value_traits().to_value_ptr(to_erase));
   }

   //! <b>Effects</b>: Erases the first element of the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased element.
   void pop_front()
   {  return this->pop_front_and_dispose(detail::null_disposer());   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the first element of the list.
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Disposer>
   void pop_front_and_dispose(Disposer disposer)
   { 
      node_ptr to_erase = node_traits::get_next(this->get_root_node());
      node_algorithms::unlink(to_erase);
      this->priv_size_traits().decrement();
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(get_real_value_traits().to_value_ptr(to_erase));
   }

   //! <b>Effects</b>: Returns a reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference front() 
   { return *get_real_value_traits().to_value_ptr(node_traits::get_next(this->get_root_node())); }

   //! <b>Effects</b>: Returns a const_reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference front() const 
   { return *get_real_value_traits().to_value_ptr(uncast(node_traits::get_next(this->get_root_node()))); }

   //! <b>Effects</b>: Returns a reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference back() 
   { return *get_real_value_traits().to_value_ptr(node_traits::get_previous(this->get_root_node())); }

   //! <b>Effects</b>: Returns a const_reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference back() const 
   { return *get_real_value_traits().to_value_ptr(uncast(node_traits::get_previous(this->get_root_node()))); }

   //! <b>Effects</b>: Returns an iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator begin() 
   { return iterator(node_traits::get_next(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const 
   { return this->cbegin(); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cbegin() const 
   { return const_iterator(node_traits::get_next(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns an iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator end() 
   { return iterator(this->get_root_node(), this); }

   //! <b>Effects</b>: Returns a const_iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator end() const 
   { return this->cend(); }

   //! <b>Effects</b>: Returns a constant iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cend() const
   { return const_iterator(uncast(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning 
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rbegin()
   { return reverse_iterator(this->end()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning 
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rbegin() const 
   { return this->crbegin(); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning 
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crbegin() const 
   { return const_reverse_iterator(end()); }

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reverse_iterator rend()   
   { return reverse_iterator(begin()); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rend() const   
   { return this->crend(); }

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crend() const   
   { return const_reverse_iterator(this->begin()); }

   //! <b>Precondition</b>: end_iterator must be a valid end iterator
   //!   of list.
   //! 
   //! <b>Effects</b>: Returns a const reference to the list associated to the end iterator
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   static list_impl &container_from_end_iterator(iterator end_iterator)
   {  return list_impl::priv_container_from_end_iterator(end_iterator);   }

   //! <b>Precondition</b>: end_iterator must be a valid end const_iterator
   //!   of list.
   //! 
   //! <b>Effects</b>: Returns a const reference to the list associated to the end iterator
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   static const list_impl &container_from_end_iterator(const_iterator end_iterator)
   {  return list_impl::priv_container_from_end_iterator(end_iterator);   }

   //! <b>Effects</b>: Returns the number of the elements contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements contained in the list.
   //!   if constant-time size option is disabled. Constant time otherwise.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   size_type size() const
   {
      if(constant_time_size)
         return this->priv_size_traits().get_size();
      else
         return node_algorithms::count(this->get_root_node()) - 1; 
   }

   //! <b>Effects</b>: Returns true if the list contains no elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   bool empty() const
   {  return node_algorithms::unique(this->get_root_node());   }

   //! <b>Effects</b>: Swaps the elements of x and *this.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void swap(list_impl& other)
   {
      node_algorithms::swap_nodes(this->get_root_node(), other.get_root_node()); 
      if(constant_time_size){
         size_type backup = this->priv_size_traits().get_size();
         this->priv_size_traits().set_size(other.priv_size_traits().get_size());
         other.priv_size_traits().set_size(backup);
      }
   }

   //! <b>Effects</b>: Moves backwards all the elements, so that the first
   //!   element becomes the second, the second becomes the third...
   //!   the last element becomes the first one.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of shifts.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void shift_backwards(size_type n = 1)
   {  node_algorithms::move_forward(this->get_root_node(), n);  }

   //! <b>Effects</b>: Moves forward all the elements, so that the second
   //!   element becomes the first, the third becomes the second...
   //!   the first element becomes the last one.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of shifts.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void shift_forward(size_type n = 1)
   {  node_algorithms::move_backwards(this->get_root_node(), n);  }

   //! <b>Effects</b>: Erases the element pointed by i of the list.
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase(const_iterator i)
   {  return this->erase_and_dispose(i, detail::null_disposer());  }

   //! <b>Requires</b>: b and e must be valid iterators to elements in *this.
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //! No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of erased elements if it's a safe-mode
   //!   or auto-unlink value, or constant-time size is enabled. Constant-time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the 
   //!   erased elements.
   iterator erase(const_iterator b, const_iterator e)
   {
      if(safemode_or_autounlink || constant_time_size){
         return this->erase_and_dispose(b, e, detail::null_disposer());
      }
      else{
         node_algorithms::unlink(b.pointed_node(), e.pointed_node());
         return e.unconst();
      }
   }

   //! <b>Requires</b>: b and e must be valid iterators to elements in *this.
   //!   n must be std::distance(b, e).
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //! No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of erased elements if it's a safe-mode
   //!   or auto-unlink value is enabled. Constant-time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the 
   //!   erased elements.
   iterator erase(const_iterator b, const_iterator e, difference_type n)
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(std::distance(b, e) == difference_type(n));
      if(safemode_or_autounlink || constant_time_size){
         return this->erase_and_dispose(b, e, detail::null_disposer());
      }
      else{
         if(constant_time_size){
            this->priv_size_traits().set_size(this->priv_size_traits().get_size() - n);
         }
         node_algorithms::unlink(b.pointed_node(), e.pointed_node());
         return e.unconst();
      }
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed by i of the list.
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //!
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template <class Disposer>
   iterator erase_and_dispose(const_iterator i, Disposer disposer)
   {
      node_ptr to_erase(i.pointed_node());
      ++i;
      node_algorithms::unlink(to_erase);
      this->priv_size_traits().decrement();
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(this->get_real_value_traits().to_value_ptr(to_erase));
      return i.unconst();
   }

   #if !defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
   template<class Disposer>
   iterator erase_and_dispose(iterator i, Disposer disposer)
   {  return this->erase_and_dispose(const_iterator(i), disposer);   }
   #endif

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element range pointed by b and e
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements erased.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased elements.
   template <class Disposer>
   iterator erase_and_dispose(const_iterator b, const_iterator e, Disposer disposer)
   {
      node_ptr bp(b.pointed_node()), ep(e.pointed_node());
      node_algorithms::unlink(bp, ep);
      while(bp != ep){
         node_ptr to_erase(bp);
         bp = node_traits::get_next(bp);
         if(safemode_or_autounlink)
            node_algorithms::init(to_erase);
         disposer(get_real_value_traits().to_value_ptr(to_erase));
         this->priv_size_traits().decrement();
      }
      return e.unconst();
   }

   //! <b>Effects</b>: Erases all the elements of the container.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //!   if it's a safe-mode or auto-unlink value_type. Constant time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased elements.
   void clear()
   {
      if(safemode_or_autounlink){
         this->clear_and_dispose(detail::null_disposer()); 
      }
      else{
         node_algorithms::init_header(this->get_root_node());
         this->priv_size_traits().set_size(size_type(0));
      }
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements of the container.
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased elements.
   template <class Disposer>
   void clear_and_dispose(Disposer disposer)
   {
      const_iterator it(this->begin()), itend(this->end());
      while(it != itend){
         node_ptr to_erase(it.pointed_node());
         ++it;
         if(safemode_or_autounlink)
            node_algorithms::init(to_erase);
         disposer(get_real_value_traits().to_value_ptr(to_erase));
      }
      node_algorithms::init_header(this->get_root_node());
      this->priv_size_traits().set_size(0);
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!   Cloner should yield to nodes equivalent to the original nodes.
   //!
   //! <b>Effects</b>: Erases all the elements from *this
   //!   calling Disposer::operator()(pointer), clones all the 
   //!   elements from src calling Cloner::operator()(const_reference )
   //!   and inserts them on *this.
   //!
   //!   If cloner throws, all cloned elements are unlinked and disposed
   //!   calling Disposer::operator()(pointer).
   //!   
   //! <b>Complexity</b>: Linear to erased plus inserted elements.
   //! 
   //! <b>Throws</b>: If cloner throws. Basic guarantee.
   template <class Cloner, class Disposer>
   void clone_from(const list_impl &src, Cloner cloner, Disposer disposer)
   {
      this->clear_and_dispose(disposer);
      detail::exception_disposer<list_impl, Disposer>
         rollback(*this, disposer);
      const_iterator b(src.begin()), e(src.end());
      for(; b != e; ++b){
         this->push_back(*cloner(*b));
      }
      rollback.release();
   }

   //! <b>Requires</b>: value must be an lvalue and p must be a valid iterator of *this.
   //!
   //! <b>Effects</b>: Inserts the value before the position pointed by p.
   //!
   //! <b>Returns</b>: An iterator to the inserted element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time. No copy constructors are called.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   iterator insert(const_iterator p, reference value)
   {
      node_ptr to_insert = this->get_real_value_traits().to_node_ptr(value);
      if(safemode_or_autounlink)
         BOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(node_algorithms::inited(to_insert));
      node_algorithms::link_before(p.pointed_node(), to_insert);
      this->priv_size_traits().increment();
      return iterator(to_insert, this);
   }

   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type and p must be a valid iterator of *this.
   //! 
   //! <b>Effects</b>: Inserts the range pointed by b and e before the position p.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   template<class Iterator>
   void insert(const_iterator p, Iterator b, Iterator e)
   {
      for (; b != e; ++b)
         this->insert(p, *b);
   }

   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Clears the list and inserts the range pointed by b and e.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus
   //!   linear to the elements contained in the list if it's a safe-mode
   //!   or auto-unlink value.
   //!   Linear to the number of elements inserted in the list otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!   to the erased elements.
   template<class Iterator>
   void assign(Iterator b, Iterator e)
   {
      this->clear();
      this->insert(this->cend(), b, e);
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Clears the list and inserts the range pointed by b and e.
   //!   No destructors or copy constructors are called.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus
   //!   linear to the elements contained in the list.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!   to the erased elements.
   template<class Iterator, class Disposer>
   void dispose_and_assign(Disposer disposer, Iterator b, Iterator e)
   {
      this->clear_and_dispose(disposer);
      this->insert(this->cend(), b, e);
   }

   //! <b>Requires</b>: p must be a valid iterator of *this.
   //!
   //! <b>Effects</b>: Transfers all the elements of list x to this list, before the
   //!   the element pointed by p. No destructors or copy constructors are called.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of
   //!    this list. Iterators of this list and all the references are not invalidated.
   void splice(const_iterator p, list_impl& x)
   {
      if(!x.empty()){
         size_traits &thist = this->priv_size_traits();
         size_traits &xt = x.priv_size_traits();
         node_algorithms::transfer
            (p.pointed_node(), x.begin().pointed_node(), x.end().pointed_node());
         thist.set_size(thist.get_size() + xt.get_size());
         xt.set_size(size_type(0));
      }
   }

   //! <b>Requires</b>: p must be a valid iterator of *this.
   //!   new_ele must point to an element contained in list x.
   //! 
   //! <b>Effects</b>: Transfers the value pointed by new_ele, from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //!   If p == new_ele or p == ++new_ele, this function is a null operation. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(const_iterator p, list_impl&x, const_iterator new_ele)
   {
      node_algorithms::transfer(p.pointed_node(), new_ele.pointed_node());
      x.priv_size_traits().decrement();
      this->priv_size_traits().increment();
   }

   //! <b>Requires</b>: p must be a valid iterator of *this.
   //!   start and end must point to elements contained in list x.
   //! 
   //! <b>Effects</b>: Transfers the range pointed by start and end from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements transferred
   //!   if constant-time size option is enabled. Constant-time otherwise.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(const_iterator p, list_impl&x, const_iterator start, const_iterator end)
   {
      if(constant_time_size)
         this->splice(p, x, start, end, std::distance(start, end));
      else
         this->splice(p, x, start, end, 1);//distance is a dummy value
   }

   //! <b>Requires</b>: p must be a valid iterator of *this.
   //!   start and end must point to elements contained in list x.
   //!   n == std::distance(start, end)
   //! 
   //! <b>Effects</b>: Transfers the range pointed by start and end from list x to this list, 
   //!   before the the element pointed by p. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(const_iterator p, list_impl&x, const_iterator start, const_iterator end, difference_type n)
   {
      if(n){
         if(constant_time_size){
            size_traits &thist = this->priv_size_traits();
            size_traits &xt = x.priv_size_traits();
            BOOST_INTRUSIVE_INVARIANT_ASSERT(n == std::distance(start, end));
            node_algorithms::transfer(p.pointed_node(), start.pointed_node(), end.pointed_node());
            thist.set_size(thist.get_size() + n);
            xt.set_size(xt.get_size() - n);
         }
         else{
            node_algorithms::transfer(p.pointed_node(), start.pointed_node(), end.pointed_node());
         }
      }
   }

   //! <b>Effects</b>: This function sorts the list *this according to std::less<value_type>. 
   //!   The sort is stable, that is, the relative order of equivalent elements is preserved.
   //! 
   //! <b>Throws</b>: If real_value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks)
   //!   or std::less<value_type> throws. Basic guarantee.
   //!
   //! <b>Notes</b>: Iterators and references are not invalidated.
   //! 
   //! <b>Complexity</b>: The number of comparisons is approximately N log N, where N
   //!   is the list's size.
   void sort() 
   {  this->sort(std::less<value_type>());  }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak ordering
   //! 
   //! <b>Effects</b>: This function sorts the list *this according to p. The sort is 
   //!   stable, that is, the relative order of equivalent elements is preserved.
   //! 
   //! <b>Throws</b>: If real_value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks)
   //!   or the predicate throws. Basic guarantee.
   //!
   //! <b>Notes</b>: This won't throw if list_base_hook<> or
   //!   list_member_hook are used.
   //!   Iterators and references are not invalidated.
   //! 
   //! <b>Complexity</b>: The number of comparisons is approximately N log N, where N
   //!   is the list's size.
   template<class Predicate>
   void sort(Predicate p)
   {
      if(node_traits::get_next(this->get_root_node()) 
         != node_traits::get_previous(this->get_root_node())){
         list_impl carry(this->priv_value_traits());
         detail::array_initializer<list_impl, 64> counter(this->priv_value_traits());
         int fill = 0;
         while(!this->empty()){
            carry.splice(carry.cbegin(), *this, this->cbegin());
            int i = 0;
            while(i < fill && !counter[i].empty()) {
               counter[i].merge(carry, p);
               carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if(i == fill)
               ++fill;
         }
         for (int i = 1; i < fill; ++i)
            counter[i].merge(counter[i-1], p);
         this->swap(counter[fill-1]);
      }
   }

   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this according to std::less<value_type>. The merge is stable; 
   //!   that is, if an element from *this is equivalent to one from x, then the element 
   //!   from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: If std::less<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void merge(list_impl& x)
   { this->merge(x, std::less<value_type>()); }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak
   //!   ordering and both *this and x must be sorted according to that ordering
   //!   The lists x and *this must be distinct. 
   //! 
   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this. The merge is stable; that is, if an element from *this is 
   //!   equivalent to one from x, then the element from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: If the predicate throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   template<class Predicate>
   void merge(list_impl& x, Predicate p)
   {
      const_iterator e(this->cend()), ex(x.cend());
      const_iterator b(this->cbegin());
      while(!x.empty()){
         const_iterator ix(x.cbegin());
         while (b != e && !p(*ix, *b)){
            ++b;
         }
         if(b == e){
            //Now transfer the rest to the end of the container
            this->splice(e, x);
            break;
         }
         else{
            size_type n(0);
            do{
               ++ix; ++n;
            } while(ix != ex && p(*ix, *b));
            this->splice(b, x, x.begin(), ix, n);
         }
      }
   }

   //! <b>Effects</b>: Reverses the order of elements in the list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void reverse()
   {  node_algorithms::reverse(this->get_root_node());   }

   //! <b>Effects</b>: Removes all the elements that compare equal to value.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   void remove(const_reference value)
   {  this->remove_if(detail::equal_to_value<const_reference>(value));  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements that compare equal to value.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Disposer>
   void remove_and_dispose(const_reference value, Disposer disposer)
   {  this->remove_and_dispose_if(detail::equal_to_value<const_reference>(value), disposer);  }

   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied. No destructors are called.
   //! 
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() calls to the predicate.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred>
   void remove_if(Pred pred)
   {  this->remove_and_dispose_if(pred, detail::null_disposer());   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //!
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred, class Disposer>
   void remove_and_dispose_if(Pred pred, Disposer disposer)
   {
      const_iterator cur(this->cbegin());
      const_iterator last(this->cend());
      while(cur != last) {
         if(pred(*cur)){
            cur = this->erase_and_dispose(cur, disposer);
         }
         else{
            ++cur;
         }
      }
   }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that are equal from the list. No destructors are called.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1 comparisons calls to pred()).
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   void unique()
   {  this->unique_and_dispose(std::equal_to<value_type>(), detail::null_disposer());  }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1 comparisons equality comparisons).
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate>
   void unique(BinaryPredicate pred)
   {  this->unique_and_dispose(pred, detail::null_disposer());  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that are equal from the list.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Disposer>
   void unique_and_dispose(Disposer disposer)
   {  this->unique_and_dispose(std::equal_to<value_type>(), disposer);  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //! 
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate, class Disposer>
   void unique_and_dispose(BinaryPredicate pred, Disposer disposer)
   {
      const_iterator itend(this->cend());
      const_iterator cur(this->cbegin());

      if(cur != itend){
         const_iterator after(cur);
         ++after;
         while(after != itend){
            if(pred(*cur, *after)){
               after = this->erase_and_dispose(after, disposer);
            }
            else{
               cur = after;
               ++after;
            }
         }
      }
   }

   //! <b>Requires</b>: value must be a reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns a const_iterator pointing to the element
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   //!   This static function is available only if the <i>value traits</i>
   //!   is stateless.
   static iterator s_iterator_to(reference value)
   {
      BOOST_STATIC_ASSERT((!stateful_value_traits));
      BOOST_INTRUSIVE_INVARIANT_ASSERT(!node_algorithms::inited(real_value_traits::to_node_ptr(value)));
      return iterator(real_value_traits::to_node_ptr(value), 0);
   }

   //! <b>Requires</b>: value must be a const reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns an iterator pointing to the element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   //!   This static function is available only if the <i>value traits</i>
   //!   is stateless.
   static const_iterator s_iterator_to(const_reference value) 
   {
      BOOST_STATIC_ASSERT((!stateful_value_traits));
      BOOST_INTRUSIVE_INVARIANT_ASSERT(!node_algorithms::inited(real_value_traits::to_node_ptr(const_cast<reference> (value))));
      return const_iterator(real_value_traits::to_node_ptr(const_cast<reference> (value)), 0);
   }

   //! <b>Requires</b>: value must be a reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns a const_iterator pointing to the element
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   iterator iterator_to(reference value)
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(!node_algorithms::inited(real_value_traits::to_node_ptr(value)));
      return iterator(real_value_traits::to_node_ptr(value), this);
   }

   //! <b>Requires</b>: value must be a const reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns an iterator pointing to the element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   const_iterator iterator_to(const_reference value) const
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(!node_algorithms::inited(real_value_traits::to_node_ptr(const_cast<reference> (value))));
      return const_iterator(real_value_traits::to_node_ptr(const_cast<reference> (value)), this);
   }

   /// @cond

   private:
   static list_impl &priv_container_from_end_iterator(const const_iterator &end_iterator)
   {
      root_plus_size *r = detail::parent_from_member<root_plus_size, node>
         ( boost::intrusive::detail::to_raw_pointer(end_iterator.pointed_node()), &root_plus_size::root_);
      data_t *d = detail::parent_from_member<data_t, root_plus_size>
         ( r, &data_t::root_plus_size_);
      list_impl *s  = detail::parent_from_member<list_impl, data_t>(d, &list_impl::data_);
      return *s;
   }
   /// @endcond
};

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator<
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{  return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());  }

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
bool operator==
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{
   typedef list_impl<Config> list_type;
   typedef typename list_type::const_iterator const_iterator;
   const bool C = list_type::constant_time_size;
   if(C && x.size() != y.size()){
      return false;
   }
   const_iterator end1 = x.end();

   const_iterator i1 = x.begin();
   const_iterator i2 = y.begin();
   if(C){
      while (i1 != end1 && *i1 == *i2) {
         ++i1;
         ++i2;
      }
      return i1 == end1;
   }
   else{
      const_iterator end2 = y.end();
      while (i1 != end1 && i2 != end2 && *i1 == *i2) {
         ++i1;
         ++i2;
      }
      return i1 == end1 && i2 == end2;
   }
}

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator!=
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{  return !(x == y); }

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator>
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{  return y < x;  }

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator<=
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{  return !(y < x);  }

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator>=
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(const list_impl<T, Options...> &x, const list_impl<T, Options...> &y)
#else
(const list_impl<Config> &x, const list_impl<Config> &y)
#endif
{  return !(x < y);  }

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class Config>
#endif
inline void swap
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
(list_impl<T, Options...> &x, list_impl<T, Options...> &y)
#else
(list_impl<Config> &x, list_impl<Config> &y)
#endif
{  x.swap(y);  }

//! Helper metafunction to define a \c list that yields to the same type when the
//! same options (either explicitly or implicitly) are used.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class ...Options>
#else
template<class T, class O1 = none, class O2 = none, class O3 = none>
#endif
struct make_list
{
   /// @cond
   typedef typename pack_options
      < list_defaults<T>, 
         #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
         O1, O2, O3
         #else
         Options...
         #endif
      >::type packed_options;

   typedef typename detail::get_value_traits
      <T, typename packed_options::value_traits>::type value_traits;

   typedef list_impl
      <
         listopt
         < value_traits
         , typename packed_options::size_type
         , packed_options::constant_time_size
         >
      > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};


#ifndef BOOST_INTRUSIVE_DOXYGEN_INVOKED

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3>
#else
template<class T, class ...Options>
#endif
class list
   :  public make_list<T, 
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3
      #else
      Options...
      #endif
   >::type
{
   typedef typename make_list
      <T, 
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3
      #else
      Options...
      #endif
      >::type      Base;
   typedef typename Base::real_value_traits     real_value_traits;
   //Assert if passed value traits are compatible with the type
   BOOST_STATIC_ASSERT((detail::is_same<typename real_value_traits::value_type, T>::value));
   BOOST_MOVABLE_BUT_NOT_COPYABLE(list)

   public:
   typedef typename Base::value_traits          value_traits;
   typedef typename Base::iterator              iterator;
   typedef typename Base::const_iterator        const_iterator;

   list(const value_traits &v_traits = value_traits())
      :  Base(v_traits)
   {}

   template<class Iterator>
   list(Iterator b, Iterator e, const value_traits &v_traits = value_traits())
      :  Base(b, e, v_traits)
   {}

   list(BOOST_RV_REF(list) x)
      :  Base(::boost::move(static_cast<Base&>(x)))
   {}

   list& operator=(BOOST_RV_REF(list) x)
   {  this->Base::operator=(::boost::move(static_cast<Base&>(x))); return *this;  }

   static list &container_from_end_iterator(iterator end_iterator)
   {  return static_cast<list &>(Base::container_from_end_iterator(end_iterator));   }

   static const list &container_from_end_iterator(const_iterator end_iterator)
   {  return static_cast<const list &>(Base::container_from_end_iterator(end_iterator));   }
};

#endif

} //namespace intrusive 
} //namespace boost 

#include "third_party/boost/boost/intrusive/detail/config_end.hpp"

#endif //BOOST_INTRUSIVE_LIST_HPP
