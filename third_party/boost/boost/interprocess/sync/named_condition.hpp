//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_CONDITION_HPP
#define BOOST_INTERPROCESS_NAMED_CONDITION_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/detail/config_begin.hpp"
#include "third_party/boost/boost/interprocess/detail/workaround.hpp"
#include "third_party/boost/boost/interprocess/creation_tags.hpp"
#include "third_party/boost/boost/interprocess/exceptions.hpp"
#include "third_party/boost/boost/interprocess/detail/interprocess_tester.hpp"
#include "third_party/boost/boost/interprocess/permissions.hpp"
#include "third_party/boost/boost/interprocess/detail/posix_time_types_wrk.hpp"
#include "third_party/boost/boost/interprocess/sync/shm/named_condition.hpp"

//!\file
//!Describes a named condition class for inter-process synchronization

namespace boost {
namespace interprocess {

/// @cond
namespace ipcdetail{ class interprocess_tester; }
/// @endcond

//! A global condition variable that can be created by name.
//! This condition variable is designed to work with named_mutex and
//! can't be placed in shared memory or memory mapped files.
class named_condition
{
   /// @cond
   //Non-copyable
   named_condition();
   named_condition(const named_condition &);
   named_condition &operator=(const named_condition &);
   /// @endcond
   public:
   //!Creates a global condition with a name.
   //!If the condition can't be created throws interprocess_exception
   named_condition(create_only_t create_only, const char *name, const permissions &perm = permissions());

   //!Opens or creates a global condition with a name. 
   //!If the condition is created, this call is equivalent to
   //!named_condition(create_only_t, ... )
   //!If the condition is already created, this call is equivalent
   //!named_condition(open_only_t, ... )
   //!Does not throw
   named_condition(open_or_create_t open_or_create, const char *name, const permissions &perm = permissions());

   //!Opens a global condition with a name if that condition is previously
   //!created. If it is not previously created this function throws
   //!interprocess_exception.
   named_condition(open_only_t open_only, const char *name);

   //!Destroys *this and indicates that the calling process is finished using
   //!the resource. The destructor function will deallocate
   //!any system resources allocated by the system for use by this process for
   //!this resource. The resource can still be opened again calling
   //!the open constructor overload. To erase the resource from the system
   //!use remove().
   ~named_condition();

   //!If there is a thread waiting on *this, change that 
   //!thread's state to ready. Otherwise there is no effect.*/
   void notify_one();

   //!Change the state of all threads waiting on *this to ready.
   //!If there are no waiting threads, notify_all() has no effect.
   void notify_all();

   //!Releases the lock on the named_mutex object associated with lock, blocks 
   //!the current thread of execution until readied by a call to 
   //!this->notify_one() or this->notify_all(), and then reacquires the lock.
   template <typename L>
   void wait(L& lock);

   //!The same as:
   //!while (!pred()) wait(lock)
   template <typename L, typename Pr>
   void wait(L& lock, Pr pred);

   //!Releases the lock on the named_mutex object associated with lock, blocks 
   //!the current thread of execution until readied by a call to 
   //!this->notify_one() or this->notify_all(), or until time abs_time is reached, 
   //!and then reacquires the lock.
   //!Returns: false if time abs_time is reached, otherwise true.
   template <typename L>
   bool timed_wait(L& lock, const boost::posix_time::ptime &abs_time);

   //!The same as:   while (!pred()) { 
   //!                  if (!timed_wait(lock, abs_time)) return pred(); 
   //!               } return true;
   template <typename L, typename Pr>
   bool timed_wait(L& lock, const boost::posix_time::ptime &abs_time, Pr pred);

   //!Erases a named condition from the system.
   //!Returns false on error. Never throws.
   static bool remove(const char *name);

   /// @cond
   private:
   ipcdetail::shm_named_condition m_cond;

   friend class ipcdetail::interprocess_tester;
   void dont_close_on_destruction()
   {  ipcdetail::interprocess_tester::dont_close_on_destruction(m_cond); }
   /// @endcond
};

/// @cond

inline named_condition::~named_condition()
{}

inline named_condition::named_condition(create_only_t, const char *name, const permissions &perm)
   :  m_cond(create_only_t(), name, perm)
{}

inline named_condition::named_condition(open_or_create_t, const char *name, const permissions &perm)
   :  m_cond(open_or_create_t(), name, perm)
{}

inline named_condition::named_condition(open_only_t, const char *name)
   :  m_cond(open_only_t(), name)
{}

inline void named_condition::notify_one()
{  m_cond.notify_one();  }

inline void named_condition::notify_all()
{  m_cond.notify_all();  }

template <typename L>
inline void named_condition::wait(L& lock)
{  m_cond.wait(lock);  }

template <typename L, typename Pr>
inline void named_condition::wait(L& lock, Pr pred)
{  m_cond.wait(lock, pred);  }

template <typename L>
inline bool named_condition::timed_wait
   (L& lock, const boost::posix_time::ptime &abs_time)
{  return m_cond.timed_wait(lock, abs_time);  }

template <typename L, typename Pr>
inline bool named_condition::timed_wait
   (L& lock, const boost::posix_time::ptime &abs_time, Pr pred)
{  return m_cond.timed_wait(lock, abs_time, pred);  }

inline bool named_condition::remove(const char *name)
{  return ipcdetail::shm_named_condition::remove(name); }

/// @endcond

}  //namespace interprocess
}  //namespace boost

#include "third_party/boost/boost/interprocess/detail/config_end.hpp"

#endif // BOOST_INTERPROCESS_NAMED_CONDITION_HPP
