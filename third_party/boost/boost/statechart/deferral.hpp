#ifndef BOOST_STATECHART_DEFERRAL_HPP_INCLUDED
#define BOOST_STATECHART_DEFERRAL_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright 2002-2006 Andreas Huber Doenni
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include "third_party/boost/boost/statechart/result.hpp"



namespace boost
{
namespace statechart
{



class event_base;

//////////////////////////////////////////////////////////////////////////////
template< class Event >
class deferral
{
  public:
    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    template< class State, class EventBase, class IdType >
    static detail::reaction_result react(
      State & stt, const EventBase &, const IdType & eventType )
    {
      if ( eventType == Event::static_type() )
      {
        return detail::result_utility::get_result( stt.defer_event() );
      }
      else
      {
        return detail::no_reaction;
      }
    }
};

template<>
class deferral< event_base >
{
  public:
    //////////////////////////////////////////////////////////////////////////
    // The following declarations should be private.
    // They are only public because many compilers lack template friends.
    //////////////////////////////////////////////////////////////////////////
    template< class State, class EventBase, class IdType >
    static detail::reaction_result react(
      State & stt, const EventBase &, const IdType & )
    {
      return detail::result_utility::get_result( stt.defer_event() );
    }
};



} // namespace statechart
} // namespace boost



#endif
