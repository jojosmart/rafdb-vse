/*
  Helper class used by variadic implementation of variadic boost::signals2::signal.

  Author: Frank Mori Hess <fmhess@users.sourceforge.net>
  Begin: 2009-05-27
*/
// Copyright Frank Mori Hess 2009
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS2_DETAIL_VARIADIC_SLOT_INVOKER_HPP
#define BOOST_SIGNALS2_DETAIL_VARIADIC_SLOT_INVOKER_HPP

#include "third_party/boost/boost/signals2/detail/variadic_arg_type.hpp"
// if compiler has variadic template support, we assume they have
// a variadic std::tuple implementation here.  We don't use boost::tuple
// because it does not have variadic template support at present.
#include <tuple>

namespace boost
{
  namespace signals2
  {
    namespace detail
    {
      template<unsigned ... values> class unsigned_meta_array {};

      template<typename UnsignedMetaArray, unsigned n> class unsigned_meta_array_appender;

      template<unsigned n, unsigned ... Args>
        class unsigned_meta_array_appender<unsigned_meta_array<Args...>, n>
      {
      public:
        typedef unsigned_meta_array<Args..., n> type;
      };

      template<unsigned n> class make_unsigned_meta_array;

      template<> class make_unsigned_meta_array<0>
      {
      public:
        typedef unsigned_meta_array<> type;
      };

      template<> class make_unsigned_meta_array<1>
      {
      public:
        typedef unsigned_meta_array<0> type;
      };

      template<unsigned n> class make_unsigned_meta_array
      {
      public:
        typedef typename unsigned_meta_array_appender<typename make_unsigned_meta_array<n-1>::type, n - 1>::type type;
      };

      template<typename R>
        class call_with_tuple_args
      {
      public:
        typedef R result_type;

        template<typename Func, typename ... Args>
          R operator()(Func &func, std::tuple<Args...> args) const
        {
          typedef typename make_unsigned_meta_array<sizeof...(Args)>::type indices_type;
          typename Func::result_type *resolver = 0;
          return m_invoke(resolver, func, indices_type(), args);
        }
      private:
        template<typename T, typename Func, unsigned ... indices, typename ... Args>
          R m_invoke(T *, Func &func, unsigned_meta_array<indices...>, std::tuple<Args...> args) const
        {
          return func(std::get<indices>(args)...);
        }
        template<typename Func, unsigned ... indices, typename ... Args>
          R m_invoke(void *, Func &func, unsigned_meta_array<indices...>, std::tuple<Args...> args) const
        {
          func(std::get<indices>(args)...);
          return R();
        }
      };

      template<typename R, typename ... Args>
        class variadic_slot_invoker
      {
      public:
        typedef R result_type;

        variadic_slot_invoker(Args & ... args): _args(args...)
        {}
        template<typename ConnectionBodyType>
          result_type operator ()(const ConnectionBodyType &connectionBody) const
        {
          result_type *resolver = 0;
          return m_invoke(connectionBody,
            resolver);
        }
      private:
        template<typename ConnectionBodyType>
        result_type m_invoke(const ConnectionBodyType &connectionBody,
          const void_type *) const
        {
          return call_with_tuple_args<result_type>()(connectionBody->slot.slot_function(), _args);
          return void_type();
        }
        template<typename ConnectionBodyType>
          result_type m_invoke(const ConnectionBodyType &connectionBody, ...) const
        {
          return call_with_tuple_args<result_type>()(connectionBody->slot.slot_function(), _args);
        }
        std::tuple<Args& ...> _args;
      };
    } // namespace detail
  } // namespace signals2
} // namespace boost


#endif // BOOST_SIGNALS2_DETAIL_VARIADIC_SLOT_INVOKER_HPP
