//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_DETAIL_SPIN_SEMAPHORE_HPP
#define BOOST_INTERPROCESS_DETAIL_SPIN_SEMAPHORE_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/interprocess/detail/config_begin.hpp"
#include "third_party/boost/boost/interprocess/detail/workaround.hpp"
#include "third_party/boost/boost/interprocess/detail/atomic.hpp"
#include "third_party/boost/boost/interprocess/detail/os_thread_functions.hpp"
#include "third_party/boost/boost/interprocess/detail/posix_time_types_wrk.hpp"
#include "third_party/boost/boost/cstdint.hpp"

namespace boost {
namespace interprocess {
namespace ipcdetail {

class spin_semaphore
{
   spin_semaphore(const spin_semaphore &);
   spin_semaphore &operator=(const spin_semaphore &);

   public:
   spin_semaphore(unsigned int initialCount);
   ~spin_semaphore();

   void post();
   void wait();
   bool try_wait();
   bool timed_wait(const boost::posix_time::ptime &abs_time);

//   int get_count() const;
   private:
   volatile boost::uint32_t m_count;
};


inline spin_semaphore::~spin_semaphore()
{}

inline spin_semaphore::spin_semaphore(unsigned int initialCount)
{  ipcdetail::atomic_write32(&this->m_count, boost::uint32_t(initialCount));  }

inline void spin_semaphore::post()
{
   ipcdetail::atomic_inc32(&m_count);
}

inline void spin_semaphore::wait()
{
   while(!ipcdetail::atomic_add_unless32(&m_count, boost::uint32_t(-1), boost::uint32_t(0))){
      while(ipcdetail::atomic_read32(&m_count) == 0){
         ipcdetail::thread_yield();
      }
   }
}

inline bool spin_semaphore::try_wait()
{
   return ipcdetail::atomic_add_unless32(&m_count, boost::uint32_t(-1), boost::uint32_t(0));
}

inline bool spin_semaphore::timed_wait(const boost::posix_time::ptime &abs_time)
{
   if(abs_time == boost::posix_time::pos_infin){
      this->wait();
      return true;
   }
   //Obtain current count and target time
   boost::posix_time::ptime now(microsec_clock::universal_time());

   do{
      if(this->try_wait()){
         break;
      }
      now = microsec_clock::universal_time();

      if(now >= abs_time){
         return this->try_wait();
      }
      // relinquish current time slice
      ipcdetail::thread_yield();
   }while (true);
   return true;
}


//inline int spin_semaphore::get_count() const
//{
   //return (int)ipcdetail::atomic_read32(&m_count);
//}

}  //namespace ipcdetail {
}  //namespace interprocess {
}  //namespace boost {

#include "third_party/boost/boost/interprocess/detail/config_end.hpp"

#endif   //BOOST_INTERPROCESS_DETAIL_SPIN_SEMAPHORE_HPP
