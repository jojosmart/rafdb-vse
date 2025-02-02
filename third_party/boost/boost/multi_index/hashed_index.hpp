/* Copyright 2003-2011 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_HASHED_INDEX_HPP
#define BOOST_MULTI_INDEX_HASHED_INDEX_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "third_party/boost/boost/config.hpp" /* keep it first to prevent nasty warns in MSVC */
#include <algorithm>
#include "third_party/boost/boost/call_traits.hpp"
#include "third_party/boost/boost/detail/allocator_utilities.hpp"
#include "third_party/boost/boost/detail/no_exceptions_support.hpp"
#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/foreach_fwd.hpp"
#include "third_party/boost/boost/limits.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/mpl/push_front.hpp"
#include "third_party/boost/boost/multi_index/detail/access_specifier.hpp"
#include "third_party/boost/boost/multi_index/detail/auto_space.hpp"
#include "third_party/boost/boost/multi_index/detail/bucket_array.hpp"
#include "third_party/boost/boost/multi_index/detail/hash_index_iterator.hpp"
#include "third_party/boost/boost/multi_index/detail/index_node_base.hpp"
#include "third_party/boost/boost/multi_index/detail/modify_key_adaptor.hpp"
#include "third_party/boost/boost/multi_index/detail/safe_ctr_proxy.hpp"
#include "third_party/boost/boost/multi_index/detail/safe_mode.hpp"
#include "third_party/boost/boost/multi_index/detail/scope_guard.hpp"
#include "third_party/boost/boost/multi_index/hashed_index_fwd.hpp"
#include "third_party/boost/boost/tuple/tuple.hpp"
#include <cstddef>
#include <functional>
#include <utility>

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#include "third_party/boost/boost/serialization/nvp.hpp"
#endif

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
#define BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT                       \
  detail::scope_guard BOOST_JOIN(check_invariant_,__LINE__)=                 \
    detail::make_obj_guard(*this,&hashed_index::check_invariant_);           \
  BOOST_JOIN(check_invariant_,__LINE__).touch();
#else
#define BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT
#endif

namespace boost{

namespace multi_index{

namespace detail{

/* hashed_index adds a layer of hashed indexing to a given Super */

/* Most of the implementation of unique and non-unique indices is
 * shared. We tell from one another on instantiation time by using
 * these tags.
 */

struct hashed_unique_tag{};
struct hashed_non_unique_tag{};

template<
  typename KeyFromValue,typename Hash,typename Pred,
  typename SuperMeta,typename TagList,typename Category
>
class hashed_index:
  BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  ,public safe_ctr_proxy_impl<
    hashed_index_iterator<
      hashed_index_node<typename SuperMeta::type::node_type>,
      bucket_array<typename SuperMeta::type::final_allocator_type> >,
    hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category> >
#else
  ,public safe_mode::safe_container<
    hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category> >
#endif
#endif

{ 
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    BOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
 * lifetime of const references bound to temporaries --precisely what
 * scopeguards are.
 */

#pragma parse_mfunc_templ off
#endif

  typedef typename SuperMeta::type                   super;

protected:
  typedef hashed_index_node<
    typename super::node_type>                       node_type;

private:
  typedef typename node_type::impl_type              node_impl_type;
  typedef typename node_impl_type::pointer           node_impl_pointer;
  typedef bucket_array<
    typename super::final_allocator_type>            bucket_array_type;

public:
  /* types */

  typedef typename KeyFromValue::result_type         key_type;
  typedef typename node_type::value_type             value_type;
  typedef KeyFromValue                               key_from_value;
  typedef Hash                                       hasher;
  typedef Pred                                       key_equal;
  typedef tuple<std::size_t,
    key_from_value,hasher,key_equal>                 ctor_args;
  typedef typename super::final_allocator_type       allocator_type;
  typedef typename allocator_type::pointer           pointer;
  typedef typename allocator_type::const_pointer     const_pointer;
  typedef typename allocator_type::reference         reference;
  typedef typename allocator_type::const_reference   const_reference;
  typedef std::size_t                                size_type;      
  typedef std::ptrdiff_t                             difference_type;

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef safe_mode::safe_iterator<
    hashed_index_iterator<
      node_type,bucket_array_type>,
    safe_ctr_proxy<
      hashed_index_iterator<
        node_type,bucket_array_type> > >             iterator;
#else
  typedef safe_mode::safe_iterator<
    hashed_index_iterator<
      node_type,bucket_array_type>,
    hashed_index>                                    iterator;
#endif
#else
  typedef hashed_index_iterator<
    node_type,bucket_array_type>                     iterator;
#endif

  typedef iterator                                   const_iterator;

  typedef iterator                                   local_iterator;
  typedef const_iterator                             const_local_iterator;
  typedef TagList                                    tag_list;

protected:
  typedef typename super::final_node_type     final_node_type;
  typedef tuples::cons<
    ctor_args, 
    typename super::ctor_args_list>           ctor_args_list;
  typedef typename mpl::push_front<
    typename super::index_type_list,
    hashed_index>::type                       index_type_list;
  typedef typename mpl::push_front<
    typename super::iterator_type_list,
    iterator>::type                           iterator_type_list;
  typedef typename mpl::push_front<
    typename super::const_iterator_type_list,
    const_iterator>::type                     const_iterator_type_list;
  typedef typename super::copy_map_type       copy_map_type;

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  typedef typename super::index_saver_type    index_saver_type;
  typedef typename super::index_loader_type   index_loader_type;
#endif

private:
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef safe_ctr_proxy_impl<
    hashed_index_iterator<
      node_type,bucket_array_type>,
    hashed_index>                             safe_super;
#else
  typedef safe_mode::safe_container<
    hashed_index>                             safe_super;
#endif
#endif

  typedef typename call_traits<value_type>::param_type value_param_type;
  typedef typename call_traits<
    key_type>::param_type                              key_param_type;

public:

  /* construct/destroy/copy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& operator=(
    const hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x)
  {
    this->final()=x.final();
    return *this;
  }

  allocator_type get_allocator()const
  {
    return this->final().get_allocator();
  }

  /* size and capacity */

  bool      empty()const{return this->final_empty_();}
  size_type size()const{return this->final_size_();}
  size_type max_size()const{return this->final_max_size_();}

  /* iterators */

  iterator begin()
  {
    return make_iterator(
      node_type::from_impl(buckets.at(first_bucket)->next()));
  }

  const_iterator begin()const
  {
    return make_iterator(
      node_type::from_impl(buckets.at(first_bucket)->next()));
  }

  iterator       end(){return make_iterator(header());}
  const_iterator end()const{return make_iterator(header());}

  const_iterator cbegin()const{return begin();}
  const_iterator cend()const{return end();}

  iterator iterator_to(const value_type& x)
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  const_iterator iterator_to(const value_type& x)const
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  /* modifiers */

  std::pair<iterator,bool> insert(value_param_type x)
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=this->final_insert_(x);
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

  iterator insert(iterator position,value_param_type x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=this->final_insert_(
      x,static_cast<final_node_type*>(position.get_node()));
    return make_iterator(p.first);
  }
    
  template<typename InputIterator>
  void insert(InputIterator first,InputIterator last)
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    iterator hint=end();
    for(;first!=last;++first)hint=insert(hint,*first);
  }

  iterator erase(iterator position)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    this->final_erase_(static_cast<final_node_type*>(position++.get_node()));
    return position;
  }

  size_type erase(key_param_type k)
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

    size_type         s=0;
    std::size_t       buc=buckets.position(hash(k));
    node_impl_pointer x=buckets.at(buc);
    node_impl_pointer y=x->next();
    while(y!=x){
      if(eq(k,key(node_type::from_impl(y)->value()))){
        bool b;
        do{
          node_impl_pointer z=y->next();
          b=z!=x&&eq(
            key(node_type::from_impl(y)->value()),
            key(node_type::from_impl(z)->value()));
          this->final_erase_(
            static_cast<final_node_type*>(node_type::from_impl(y)));
          y=z;
          ++s;
        }while(b);
        break;
      }
      y=y->next();
    }
    return s;
  }

  iterator erase(iterator first,iterator last)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(first,*this);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(last,*this);
    BOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    while(first!=last){
      first=erase(first);
    }
    return first;
  }

  bool replace(iterator position,value_param_type x)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    return this->final_replace_(
      x,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier>
  bool modify(iterator position,Modifier mod)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return this->final_modify_(
      mod,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier,typename Rollback>
  bool modify(iterator position,Modifier mod,Rollback back)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return this->final_modify_(
      mod,back,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier>
  bool modify_key(iterator position,Modifier mod)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    return modify(
      position,modify_key_adaptor<Modifier,value_type,KeyFromValue>(mod,key));
  }

  template<typename Modifier,typename Rollback>
  bool modify_key(iterator position,Modifier mod,Rollback back)
  {
    BOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    return modify(
      position,
      modify_key_adaptor<Modifier,value_type,KeyFromValue>(mod,key),
      modify_key_adaptor<Rollback,value_type,KeyFromValue>(back,key));
  }

  void clear()
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    this->final_clear_();
  }

  void swap(hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x)
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    this->final_swap_(x.final());
  }

  /* observers */

  key_from_value key_extractor()const{return key;}
  hasher         hash_function()const{return hash;}
  key_equal      key_eq()const{return eq;}
  
  /* lookup */

  /* Internally, these ops rely on const_iterator being the same
   * type as iterator.
   */

  template<typename CompatibleKey>
  iterator find(const CompatibleKey& k)const
  {
    return find(k,hash,eq);
  }

  template<
    typename CompatibleKey,typename CompatibleHash,typename CompatiblePred
  >
  iterator find(
    const CompatibleKey& k,
    const CompatibleHash& hash,const CompatiblePred& eq)const
  {
    std::size_t       buc=buckets.position(hash(k));
    node_impl_pointer x=buckets.at(buc);
    node_impl_pointer y=x->next();
    while(y!=x){
      if(eq(k,key(node_type::from_impl(y)->value()))){
        return make_iterator(node_type::from_impl(y));
      }
      y=y->next();
    }
    return end();
  }

  template<typename CompatibleKey>
  size_type count(const CompatibleKey& k)const
  {
    return count(k,hash,eq);
  }

  template<
    typename CompatibleKey,typename CompatibleHash,typename CompatiblePred
  >
  size_type count(
    const CompatibleKey& k,
    const CompatibleHash& hash,const CompatiblePred& eq)const
  {
    size_type         res=0;
    std::size_t       buc=buckets.position(hash(k));
    node_impl_pointer x=buckets.at(buc);
    node_impl_pointer y=x->next();
    while(y!=x){
      if(eq(k,key(node_type::from_impl(y)->value()))){
        do{
          ++res;
          y=y->next();
        }while(y!=x&&eq(k,key(node_type::from_impl(y)->value())));
        break;
      }
      y=y->next();
    }
    return res;
  }

  template<typename CompatibleKey>
  std::pair<iterator,iterator> equal_range(const CompatibleKey& k)const
  {
    return equal_range(k,hash,eq);
  }

  template<
    typename CompatibleKey,typename CompatibleHash,typename CompatiblePred
  >
  std::pair<iterator,iterator> equal_range(
    const CompatibleKey& k,
    const CompatibleHash& hash,const CompatiblePred& eq)const
  {
    std::size_t       buc=buckets.position(hash(k));
    node_impl_pointer x=buckets.at(buc);
    node_impl_pointer y=x->next();
    while(y!=x){
      if(eq(k,key(node_type::from_impl(y)->value()))){
        node_impl_pointer y0=y;
        do{
          y=y->next();
        }while(y!=x&&eq(k,key(node_type::from_impl(y)->value())));
        if(y==x){
          do{
            ++y;
          }while(y==y->next());
          y=y->next();
        }
        return std::pair<iterator,iterator>(
          make_iterator(node_type::from_impl(y0)),
          make_iterator(node_type::from_impl(y)));
      }
      y=y->next();
    }
    return std::pair<iterator,iterator>(end(),end());
  }

  /* bucket interface */

  size_type bucket_count()const{return buckets.size();}
  size_type max_bucket_count()const{return static_cast<size_type>(-1);}

  size_type bucket_size(size_type n)const
  {
    size_type         res=0;
    node_impl_pointer x=buckets.at(n);
    node_impl_pointer y=x->next();
    while(y!=x){
      ++res;
      y=y->next();
    }
    return res;
  }

  size_type bucket(key_param_type k)const
  {
    return buckets.position(hash(k));
  }

  local_iterator begin(size_type n)
  {
    return const_cast<const hashed_index*>(this)->begin(n);
  }

  const_local_iterator begin(size_type n)const
  {
    node_impl_pointer x=buckets.at(n);
    node_impl_pointer y=x->next();
    if(y==x)return end();
    return make_iterator(node_type::from_impl(y));
  }

  local_iterator end(size_type n)
  {
    return const_cast<const hashed_index*>(this)->end(n);
  }

  const_local_iterator end(size_type n)const
  {
    node_impl_pointer x=buckets.at(n);
    if(x==x->next())return end();
    do{
      ++x;
    }while(x==x->next());
    return make_iterator(node_type::from_impl(x->next()));
  }

  const_local_iterator cbegin(size_type n)const{return begin(n);}
  const_local_iterator cend(size_type n)const{return end(n);}

  local_iterator local_iterator_to(const value_type& x)
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  const_local_iterator local_iterator_to(const value_type& x)const
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  /* hash policy */

  float load_factor()const{return static_cast<float>(size())/bucket_count();}
  float max_load_factor()const{return mlf;}
  void  max_load_factor(float z){mlf=z;calculate_max_load();}

  void rehash(size_type n)
  {
    BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT;
    if(size()<max_load&&n<=bucket_count())return;

    size_type bc =(std::numeric_limits<size_type>::max)();
    float     fbc=static_cast<float>(1+size()/mlf);
    if(bc>fbc){
      bc=static_cast<size_type>(fbc);
      if(bc<n)bc=n;
    }
    unchecked_rehash(bc);
  }

BOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  hashed_index(const ctor_args_list& args_list,const allocator_type& al):
    super(args_list.get_tail(),al),
    key(tuples::get<1>(args_list.get_head())),
    hash(tuples::get<2>(args_list.get_head())),
    eq(tuples::get<3>(args_list.get_head())),
    buckets(al,header()->impl(),tuples::get<0>(args_list.get_head())),
    mlf(1.0f),
    first_bucket(buckets.size())
  {
    calculate_max_load();
  }

  hashed_index(
    const hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x):
    super(x),

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super(),
#endif

    key(x.key),
    hash(x.hash),
    eq(x.eq),
    buckets(x.get_allocator(),header()->impl(),x.buckets.size()),
    mlf(x.mlf),
    max_load(x.max_load),
    first_bucket(x.first_bucket)
  {
    /* Copy ctor just takes the internal configuration objects from x. The rest
     * is done in subsequent call to copy_().
     */
  }

  ~hashed_index()
  {
    /* the container is guaranteed to be empty by now */
  }

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  iterator make_iterator(node_type* node)
  {
    return iterator(node,&buckets,this);
  }

  const_iterator make_iterator(node_type* node)const
  {
    return const_iterator(
      node,
      &const_cast<bucket_array_type&>(buckets),
      const_cast<hashed_index*>(this));
  }
#else
  iterator make_iterator(node_type* node)
  {
    return iterator(node,&buckets);
  }

  const_iterator make_iterator(node_type* node)const
  {
    return const_iterator(node,&const_cast<bucket_array_type&>(buckets));
  }
#endif

  void copy_(
    const hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x,
    const copy_map_type& map)
  {
    for(node_impl_pointer begin_org=x.buckets.begin(),
                          begin_cpy=buckets.begin(),
                          end_org=x.buckets.end();
        begin_org!=end_org;++begin_org,++begin_cpy){

      node_impl_pointer next_org=begin_org->next();
      node_impl_pointer cpy=begin_cpy;
      while(next_org!=begin_org){
        cpy->next()=
          static_cast<node_type*>(
            map.find(
              static_cast<final_node_type*>(
                node_type::from_impl(next_org))))->impl();
        next_org=next_org->next();
        cpy=cpy->next();
      }
      cpy->next()=begin_cpy;
    }

    super::copy_(x,map);
  }

  node_type* insert_(value_param_type v,node_type* x)
  {
    reserve(size()+1);

    std::size_t       buc=find_bucket(v);
    node_impl_pointer pos=buckets.at(buc);
    if(!link_point(v,pos,Category()))return node_type::from_impl(pos);

    node_type* res=static_cast<node_type*>(super::insert_(v,x));
    if(res==x){
      link(x,pos);
      if(first_bucket>buc)first_bucket=buc;
    }
    return res;
  }

  node_type* insert_(value_param_type v,node_type* position,node_type* x)
  {
    reserve(size()+1);

    std::size_t       buc=find_bucket(v);
    node_impl_pointer pos=buckets.at(buc);
    if(!link_point(v,pos,Category()))return node_type::from_impl(pos);

    node_type* res=static_cast<node_type*>(super::insert_(v,position,x));
    if(res==x){
      link(x,pos);
      if(first_bucket>buc)first_bucket=buc;
    }
    return res;
  }

  void erase_(node_type* x)
  {
    unlink(x);
    first_bucket=buckets.first_nonempty(first_bucket);
    super::erase_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    detach_iterators(x);
#endif
  }

  void delete_all_nodes_()
  {
    for(node_impl_pointer x=buckets.begin(),x_end=buckets.end();
        x!=x_end;++x){
      node_impl_pointer y=x->next();
      while(y!=x){
        node_impl_pointer z=y->next();
        this->final_delete_node_(
          static_cast<final_node_type*>(node_type::from_impl(y)));
        y=z;
      }
    }
  }

  void clear_()
  {
    super::clear_();
    buckets.clear();
    first_bucket=buckets.size();

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::detach_dereferenceable_iterators();
#endif
  }

  void swap_(
    hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x)
  {
    std::swap(key,x.key);
    std::swap(hash,x.hash);
    std::swap(eq,x.eq);
    buckets.swap(x.buckets);
    std::swap(mlf,x.mlf);
    std::swap(max_load,x.max_load);
    std::swap(first_bucket,x.first_bucket);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::swap(x);
#endif

    super::swap_(x);
  }

  bool replace_(value_param_type v,node_type* x)
  {
    if(eq(key(v),key(x->value()))){
      return super::replace_(v,x);
    }

    node_impl_pointer y=prev(x);
    unlink_next(y);

    BOOST_TRY{
      std::size_t       buc=find_bucket(v);
      node_impl_pointer pos=buckets.at(buc);
      if(link_point(v,pos,Category())&&super::replace_(v,x)){
        link(x,pos);
        if(first_bucket>buc){
          first_bucket=buc;
        }
        else if(first_bucket<buc){
          first_bucket=buckets.first_nonempty(first_bucket);
        }
        return true;
      }
      link(x,y);
      return false;
    }
    BOOST_CATCH(...){
      link(x,y);
      BOOST_RETHROW;
    }
    BOOST_CATCH_END
  }

  bool modify_(node_type* x)
  {
    std::size_t buc;
    bool        b; 
    BOOST_TRY{
      buc=find_bucket(x->value());
      b=in_place(x->impl(),key(x->value()),buc,Category());
    }
    BOOST_CATCH(...){
      erase_(x);
      BOOST_RETHROW;
    }
    BOOST_CATCH_END
    if(!b){
      unlink(x);
      BOOST_TRY{
        node_impl_pointer pos=buckets.at(buc);
        if(!link_point(x->value(),pos,Category())){
          first_bucket=buckets.first_nonempty(first_bucket);
          super::erase_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
          detach_iterators(x);
#endif
          return false;
        }
        link(x,pos);
        if(first_bucket>buc){
          first_bucket=buc;
        }
        else if(first_bucket<buc){
          first_bucket=buckets.first_nonempty(first_bucket);
        }
      }
      BOOST_CATCH(...){
        first_bucket=buckets.first_nonempty(first_bucket);
        super::erase_(x);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
      detach_iterators(x);
#endif

        BOOST_RETHROW;
      }
      BOOST_CATCH_END
    }

    BOOST_TRY{
      if(!super::modify_(x)){
        unlink(x);
        first_bucket=buckets.first_nonempty(first_bucket);
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
        detach_iterators(x);
#endif
        return false;
      }
      else return true;
    }
    BOOST_CATCH(...){
      unlink(x);
      first_bucket=buckets.first_nonempty(first_bucket);

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
      detach_iterators(x);
#endif

      BOOST_RETHROW;
    }
    BOOST_CATCH_END
  }

  bool modify_rollback_(node_type* x)
  {
    std::size_t buc=find_bucket(x->value());
    if(in_place(x->impl(),key(x->value()),buc,Category())){
      return super::modify_rollback_(x);
    }

    node_impl_pointer y=prev(x);
    unlink_next(y);

    BOOST_TRY{
      node_impl_pointer pos=buckets.at(buc);
      if(link_point(x->value(),pos,Category())&&super::modify_rollback_(x)){
        link(x,pos);
        if(first_bucket>buc){
          first_bucket=buc;
        }
        else if(first_bucket<buc){
          first_bucket=buckets.first_nonempty(first_bucket);
        }
        return true;
      }
      link(x,y);
      return false;
    }
    BOOST_CATCH(...){
      link(x,y);
      BOOST_RETHROW;
    }
    BOOST_CATCH_END
  }

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  /* serialization */

  template<typename Archive>
  void save_(
    Archive& ar,const unsigned int version,const index_saver_type& sm)const
  {
    ar<<serialization::make_nvp("position",buckets);
    super::save_(ar,version,sm);
  }

  template<typename Archive>
  void load_(Archive& ar,const unsigned int version,const index_loader_type& lm)
  {
    ar>>serialization::make_nvp("position",buckets);
    super::load_(ar,version,lm);
  }
#endif

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    if(size()==0||begin()==end()){
      if(size()!=0||begin()!=end())return false;
    }
    else{
      size_type s0=0;
      for(const_iterator it=begin(),it_end=end();it!=it_end;++it,++s0){}
      if(s0!=size())return false;

      size_type s1=0;
      for(size_type buc=0;buc<bucket_count();++buc){
        size_type ss1=0;
        for(const_local_iterator it=begin(buc),it_end=end(buc);
            it!=it_end;++it,++ss1){
          if(find_bucket(*it)!=buc)return false;
        }
        if(ss1!=bucket_size(buc))return false;
        s1+=ss1;
      }
      if(s1!=size())return false;
    }

    if(first_bucket!=buckets.first_nonempty(0))return false;

    return super::invariant_();
  }

  /* This forwarding function eases things for the boost::mem_fn construct
   * in BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT. Actually,
   * final_check_invariant is already an inherited member function of index.
   */
  void check_invariant_()const{this->final_check_invariant_();}
#endif

private:
  node_type* header()const{return this->final_header();}

  std::size_t find_bucket(value_param_type v)const
  {
    return bucket(key(v));
  }

  bool link_point(
    value_param_type v,node_impl_pointer& pos,hashed_unique_tag)
  {
    node_impl_pointer x=pos->next();
    while(x!=pos){
      if(eq(key(v),key(node_type::from_impl(x)->value()))){
        pos=x;
        return false;
      }
      x=x->next();
    }
    return true;
  }

  bool link_point(
    value_param_type v,node_impl_pointer& pos,hashed_non_unique_tag)
  {
    node_impl_pointer prev=pos;
    node_impl_pointer x=pos->next();
    while(x!=pos){
      if(eq(key(v),key(node_type::from_impl(x)->value()))){
        pos=prev;
        return true;
      }
      prev=x;
      x=x->next();
    }
    return true;
  }
  
  static void link(node_type* x,node_impl_pointer pos)
  {
    node_impl_type::link(x->impl(),pos);
  };

  static void link(node_impl_pointer x,node_impl_pointer pos)
  {
    node_impl_type::link(x,pos);
  };

  static void unlink(node_type* x)
  {
    node_impl_type::unlink(x->impl());
  };

  static node_impl_pointer prev(node_type* x)
  {
    return node_impl_type::prev(x->impl());
  }

  static node_impl_pointer prev_from(node_type* x,node_impl_pointer y)
  {
    return node_impl_type::prev_from(x->impl(),y);
  }

  static void unlink_next(node_impl_pointer x)
  {
    node_impl_type::unlink_next(x);
  }

  void calculate_max_load()
  {
    float fml=static_cast<float>(mlf*bucket_count());
    max_load=(std::numeric_limits<size_type>::max)();
    if(max_load>fml)max_load=static_cast<size_type>(fml);
  }

  void reserve(size_type n)
  {
    if(n>max_load){
      size_type bc =(std::numeric_limits<size_type>::max)();
      float     fbc=static_cast<float>(1+n/mlf);
      if(bc>fbc)bc =static_cast<size_type>(fbc);
      unchecked_rehash(bc);
    }
  }

  void unchecked_rehash(size_type n)
  {
    bucket_array_type buckets1(get_allocator(),header()->impl(),n);
    auto_space<std::size_t,allocator_type> hashes(get_allocator(),size());

    std::size_t i=0;
    node_impl_pointer x=buckets.begin();
    node_impl_pointer x_end=buckets.end();
    for(;x!=x_end;++x){
      node_impl_pointer y=x->next();
      while(y!=x){
        hashes.data()[i++]=hash(key(node_type::from_impl(y)->value()));
        y=y->next();
      }
    }

    i=0;
    x=buckets.begin();
    for(;x!=x_end;++x){
      node_impl_pointer y=x->next();
      while(y!=x){
        node_impl_pointer z=y->next();
        std::size_t       buc1=buckets1.position(hashes.data()[i++]);
        node_impl_pointer x1=buckets1.at(buc1);
        link(y,x1);
        y=z;
      }
    }

    buckets.swap(buckets1);
    calculate_max_load();
    first_bucket=buckets.first_nonempty(0);
  }

  bool in_place(
    node_impl_pointer x,key_param_type k,std::size_t buc,
    hashed_unique_tag)const
  {
    std::less_equal<node_impl_pointer> leq;
    node_impl_pointer                  bbegin=buckets.begin();
    node_impl_pointer                  bend=buckets.end();
    node_impl_pointer                  pbuc=x->next();

    while(!leq(bbegin,pbuc)||!leq(pbuc,bend))pbuc=pbuc->next();
    if(buc!=static_cast<std::size_t>(pbuc-bbegin))return false;

    node_impl_pointer y=x;
    while(y->next()!=x){
      y=y->next();
      if(y==pbuc)continue;
      if(eq(k,key(node_type::from_impl(y)->value())))return false;
    }
    return true;
  }

  bool in_place(
    node_impl_pointer x,key_param_type k,std::size_t buc,
    hashed_non_unique_tag)const
  {
    std::less_equal<node_impl_pointer> leq;
    node_impl_pointer                  bbegin=buckets.begin();
    node_impl_pointer                  bend=buckets.end();
    node_impl_pointer                  pbuc=x->next();

    while(!leq(bbegin,pbuc)||!leq(pbuc,bend))pbuc=pbuc->next();
    if(buc!=static_cast<std::size_t>(pbuc-bbegin))return false;

    node_impl_pointer y=x->next();
    if(y!=pbuc){
      if(eq(k,key(node_type::from_impl(y)->value()))){
        /* adjacent to equivalent element -> in place */
        return true;
      }
      else{
        y=y->next();
        while(y!=pbuc){
          if(eq(k,key(node_type::from_impl(y)->value())))return false;
          y=y->next();
        }
      }
    }
    while(y->next()!=x){
      y=y->next();
      if(eq(k,key(node_type::from_impl(y)->value()))){
        while(y->next()!=x){
          y=y->next();
          if(!eq(k,key(node_type::from_impl(y)->value())))return false;
        }
        /* after a group of equivalent elements --> in place */
        return true;
      }
    }
    return true;
  }


#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  void detach_iterators(node_type* x)
  {
    iterator it=make_iterator(x);
    safe_mode::detach_equivalent_iterators(it);
  }
#endif

  key_from_value               key;
  hasher                       hash;
  key_equal                    eq;
  bucket_array_type            buckets;
  float                        mlf;
  size_type                    max_load;
  std::size_t                  first_bucket;
      
#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    BOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif
};

/*  specialized algorithms */

template<
  typename KeyFromValue,typename Hash,typename Pred,
  typename SuperMeta,typename TagList,typename Category
>
void swap(
  hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& x,
  hashed_index<KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>& y)
{
  x.swap(y);
}

} /* namespace multi_index::detail */

/* hashed index specifiers */

template<typename Arg1,typename Arg2,typename Arg3,typename Arg4>
struct hashed_unique
{
  typedef typename detail::hashed_index_args<
    Arg1,Arg2,Arg3,Arg4>                           index_args;
  typedef typename index_args::tag_list_type::type tag_list_type;
  typedef typename index_args::key_from_value_type key_from_value_type;
  typedef typename index_args::hash_type           hash_type;
  typedef typename index_args::pred_type           pred_type;

  template<typename Super>
  struct node_class
  {
    typedef detail::hashed_index_node<Super> type;
  };

  template<typename SuperMeta>
  struct index_class
  {
    typedef detail::hashed_index<
      key_from_value_type,hash_type,pred_type,
      SuperMeta,tag_list_type,detail::hashed_unique_tag> type;
  };
};

template<typename Arg1,typename Arg2,typename Arg3,typename Arg4>
struct hashed_non_unique
{
  typedef typename detail::hashed_index_args<
    Arg1,Arg2,Arg3,Arg4>                           index_args;
  typedef typename index_args::tag_list_type::type tag_list_type;
  typedef typename index_args::key_from_value_type key_from_value_type;
  typedef typename index_args::hash_type           hash_type;
  typedef typename index_args::pred_type           pred_type;

  template<typename Super>
  struct node_class
  {
    typedef detail::hashed_index_node<Super> type;
  };

  template<typename SuperMeta>
  struct index_class
  {
    typedef detail::hashed_index<
      key_from_value_type,hash_type,pred_type,
      SuperMeta,tag_list_type,detail::hashed_non_unique_tag> type;
  };
};

} /* namespace multi_index */

} /* namespace boost */

/* Boost.Foreach compatibility */

template<
  typename KeyFromValue,typename Hash,typename Pred,
  typename SuperMeta,typename TagList,typename Category
>
inline boost::mpl::true_* boost_foreach_is_noncopyable(
  boost::multi_index::detail::hashed_index<
    KeyFromValue,Hash,Pred,SuperMeta,TagList,Category>*&,
  boost::foreach::tag)
{
  return 0;
}

#undef BOOST_MULTI_INDEX_HASHED_INDEX_CHECK_INVARIANT

#endif
