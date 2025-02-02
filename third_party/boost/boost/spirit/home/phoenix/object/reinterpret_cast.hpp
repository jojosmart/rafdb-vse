/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OBJECT_REINTERPRET_CAST_HPP
#define PHOENIX_OBJECT_REINTERPRET_CAST_HPP

#include "third_party/boost/boost/spirit/home/phoenix/core/compose.hpp"

namespace boost { namespace phoenix
{
    namespace impl
    {
        template <typename T>
        struct reinterpret_cast_eval
        {
            template <typename Env, typename U>
            struct result
            {
                typedef T type;
            };

            template <typename RT, typename Env, typename U>
            static RT
            eval(Env const& env, U& obj)
            {
                return reinterpret_cast<RT>(obj.eval(env));
            }
        };
    }

    template <typename T, typename U>
    inline actor<typename as_composite<impl::reinterpret_cast_eval<T>, U>::type>
    reinterpret_cast_(U const& obj)
    {
        return compose<impl::reinterpret_cast_eval<T> >(obj);
    }
}}

#endif
