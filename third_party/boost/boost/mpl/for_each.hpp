
#ifndef BOOST_MPL_FOR_EACH_HPP_INCLUDED
#define BOOST_MPL_FOR_EACH_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: for_each.hpp 55648 2009-08-18 05:16:53Z agurtovoy $
// $Date: 2009-08-18 01:16:53 -0400 (Tue, 18 Aug 2009) $
// $Revision: 55648 $

#include "third_party/boost/boost/mpl/is_sequence.hpp"
#include "third_party/boost/boost/mpl/begin_end.hpp"
#include "third_party/boost/boost/mpl/apply.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/mpl/next_prior.hpp"
#include "third_party/boost/boost/mpl/deref.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"
#include "third_party/boost/boost/mpl/assert.hpp"
#include "third_party/boost/boost/mpl/aux_/unwrap.hpp"

#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/utility/value_init.hpp"

namespace boost { namespace mpl {

namespace aux {

template< bool done = true >
struct for_each_impl
{
    template<
          typename Iterator
        , typename LastIterator
        , typename TransformFunc
        , typename F
        >
    static void execute(
          Iterator*
        , LastIterator*
        , TransformFunc*
        , F
        )
    {
    }
};

template<>
struct for_each_impl<false>
{
    template<
          typename Iterator
        , typename LastIterator
        , typename TransformFunc
        , typename F
        >
    static void execute(
          Iterator*
        , LastIterator*
        , TransformFunc* 
        , F f
        )
    {
        typedef typename deref<Iterator>::type item;
        typedef typename apply1<TransformFunc,item>::type arg;
    
        // dwa 2002/9/10 -- make sure not to invoke undefined behavior
        // when we pass arg.
        value_initialized<arg> x;
        aux::unwrap(f, 0)(boost::get(x));
        
        typedef typename mpl::next<Iterator>::type iter;
        for_each_impl<boost::is_same<iter,LastIterator>::value>
            ::execute( static_cast<iter*>(0), static_cast<LastIterator*>(0), static_cast<TransformFunc*>(0), f);
    }
};

} // namespace aux

// agurt, 17/mar/02: pointer default parameters are necessary to workaround 
// MSVC 6.5 function template signature's mangling bug
template<
      typename Sequence
    , typename TransformOp
    , typename F
    >
inline
void for_each(F f, Sequence* = 0, TransformOp* = 0)
{
    BOOST_MPL_ASSERT(( is_sequence<Sequence> ));

    typedef typename begin<Sequence>::type first;
    typedef typename end<Sequence>::type last;

    aux::for_each_impl< boost::is_same<first,last>::value >
        ::execute(static_cast<first*>(0), static_cast<last*>(0), static_cast<TransformOp*>(0), f);
}

template<
      typename Sequence
    , typename F
    >
inline
void for_each(F f, Sequence* = 0)
{
    for_each<Sequence, identity<> >(f);
}

}}

#endif // BOOST_MPL_FOR_EACH_HPP_INCLUDED
