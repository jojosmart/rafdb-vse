#ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_PT_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_PT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  detail/sp_counted_base_pt.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "third_party/boost/boost/detail/sp_typeinfo.hpp"
#include <pthread.h>

namespace boost
{

namespace detail
{

class sp_counted_base
{
private:

    sp_counted_base( sp_counted_base const & );
    sp_counted_base & operator= ( sp_counted_base const & );

    long use_count_;        // #shared
    long weak_count_;       // #weak + (#shared != 0)

    mutable pthread_mutex_t m_;

public:

    sp_counted_base(): use_count_( 1 ), weak_count_( 1 )
    {
// HPUX 10.20 / DCE has a nonstandard pthread_mutex_init

#if defined(__hpux) && defined(_DECTHREADS_)
        pthread_mutex_init( &m_, pthread_mutexattr_default );
#else
        pthread_mutex_init( &m_, 0 );
#endif
    }

    virtual ~sp_counted_base() // nothrow
    {
        pthread_mutex_destroy( &m_ );
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destroy() is called when weak_count_ drops to zero.

    virtual void destroy() // nothrow
    {
        delete this;
    }

    virtual void * get_deleter( sp_typeinfo const & ti ) = 0;

    void add_ref_copy()
    {
        pthread_mutex_lock( &m_ );
        ++use_count_;
        pthread_mutex_unlock( &m_ );
    }

    bool add_ref_lock() // true on success
    {
        pthread_mutex_lock( &m_ );
        bool r = use_count_ == 0? false: ( ++use_count_, true );
        pthread_mutex_unlock( &m_ );
        return r;
    }

    void release() // nothrow
    {
        pthread_mutex_lock( &m_ );
        long new_use_count = --use_count_;
        pthread_mutex_unlock( &m_ );

        if( new_use_count == 0 )
        {
            dispose();
            weak_release();
        }
    }

    void weak_add_ref() // nothrow
    {
        pthread_mutex_lock( &m_ );
        ++weak_count_;
        pthread_mutex_unlock( &m_ );
    }

    void weak_release() // nothrow
    {
        pthread_mutex_lock( &m_ );
        long new_weak_count = --weak_count_;
        pthread_mutex_unlock( &m_ );

        if( new_weak_count == 0 )
        {
            destroy();
        }
    }

    long use_count() const // nothrow
    {
        pthread_mutex_lock( &m_ );
        long r = use_count_;
        pthread_mutex_unlock( &m_ );

        return r;
    }
};

} // namespace detail

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_PT_HPP_INCLUDED
