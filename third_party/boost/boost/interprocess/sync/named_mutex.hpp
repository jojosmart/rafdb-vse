//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_MUTEX_HPP
#define BOOST_INTERPROCESS_NAMED_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/detail/config_begin.hpp"
#include "third_party/boost/boost/interprocess/detail/workaround.hpp"
#include "third_party/boost/boost/interprocess/creation_tags.hpp"
#include "third_party/boost/boost/interprocess/exceptions.hpp"
#include "third_party/boost/boost/interprocess/detail/interprocess_tester.hpp"
#include "third_party/boost/boost/interprocess/detail/posix_time_types_wrk.hpp"
#include "third_party/boost/boost/interprocess/permissions.hpp"

#if defined(BOOST_INTERPROCESS_NAMED_MUTEX_USES_POSIX_SEMAPHORES)
#include "third_party/boost/boost/interprocess/sync/posix/named_mutex.hpp"
#else
#include "third_party/boost/boost/interprocess/sync/shm/named_mutex.hpp"
#endif

//!\file
//!Describes a named mutex class for inter-process synchronization

namespace boost {
namespace interprocess {

class named_condition;

//!A mutex with a global name, so it can be found from different 
//!processes. This mutex can't be placed in shared memory, and
//!each process should have it's own named_mutex.
class named_mutex
{
   /// @cond

   //Non-copyable
   named_mutex();
   named_mutex(const named_mutex &);
   named_mutex &operator=(const named_mutex &);
   friend class named_condition;
   /// @endcond

   public:
   //!Creates a global interprocess_mutex with a name.
   //!Throws interprocess_exception on error.
   named_mutex(create_only_t create_only, const char *name, const permissions &perm = permissions());

   //!Opens or creates a global mutex with a name. 
   //!If the mutex is created, this call is equivalent to
   //!named_mutex(create_only_t, ... )
   //!If the mutex is already created, this call is equivalent
   //!named_mutex(open_only_t, ... )
   //!Does not throw
   named_mutex(open_or_create_t open_or_create, const char *name, const permissions &perm = permissions());

   //!Opens a global mutex with a name if that mutex is previously
   //!created. If it is not previously created this function throws
   //!interprocess_exception.
   named_mutex(open_only_t open_only, const char *name);

   //!Destroys *this and indicates that the calling process is finished using
   //!the resource. The destructor function will deallocate
   //!any system resources allocated by the system for use by this process for
   //!this resource. The resource can still be opened again calling
   //!the open constructor overload. To erase the resource from the system
   //!use remove().
   ~named_mutex();

   //!Unlocks a previously locked
   //!interprocess_mutex.
   void unlock();

   //!Locks interprocess_mutex, sleeps when interprocess_mutex is already locked.
   //!Throws interprocess_exception if a severe error is found
   void lock();

   //!Tries to lock the interprocess_mutex, returns false when interprocess_mutex 
   //!is already locked, returns true when success.
   //!Throws interprocess_exception if a severe error is found
   bool try_lock();

   //!Tries to lock the interprocess_mutex until time abs_time,
   //!Returns false when timeout expires, returns true when locks.
   //!Throws interprocess_exception if a severe error is found
   bool timed_lock(const boost::posix_time::ptime &abs_time);

   //!Erases a named mutex from the system.
   //!Returns false on error. Never throws.
   static bool remove(const char *name);

   /// @cond
   private:
   friend class ipcdetail::interprocess_tester;
   void dont_close_on_destruction();

   #if defined(BOOST_INTERPROCESS_NAMED_MUTEX_USES_POSIX_SEMAPHORES)
   typedef ipcdetail::posix_named_mutex   impl_t;
   impl_t m_mut;
   #else
   typedef ipcdetail::shm_named_mutex     impl_t;
   impl_t m_mut;
   public:
   interprocess_mutex *mutex() const
   {  return m_mut.mutex(); }
   #endif

   /// @endcond
};

/// @cond

inline named_mutex::named_mutex(create_only_t, const char *name, const permissions &perm)
   :  m_mut(create_only_t(), name, perm)
{}

inline named_mutex::named_mutex(open_or_create_t, const char *name, const permissions &perm)
   :  m_mut(open_or_create_t(), name, perm)
{}

inline named_mutex::named_mutex(open_only_t, const char *name)
   :  m_mut(open_only_t(), name)
{}

inline void named_mutex::dont_close_on_destruction()
{  ipcdetail::interprocess_tester::dont_close_on_destruction(m_mut); }

inline named_mutex::~named_mutex()
{}

inline void named_mutex::lock()
{  m_mut.lock();  }

inline void named_mutex::unlock()
{  m_mut.unlock();  }

inline bool named_mutex::try_lock()
{  return m_mut.try_lock();  }

inline bool named_mutex::timed_lock(const boost::posix_time::ptime &abs_time)
{  return m_mut.timed_lock(abs_time);  }

inline bool named_mutex::remove(const char *name)
{  return impl_t::remove(name);   }

/// @endcond

}  //namespace interprocess {
}  //namespace boost {

#include "third_party/boost/boost/interprocess/detail/config_end.hpp"

#endif   //BOOST_INTERPROCESS_NAMED_MUTEX_HPP
