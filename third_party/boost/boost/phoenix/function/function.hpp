/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PHOENIX_FUNCTION_FUNCTION_HPP
#define BOOST_PHOENIX_FUNCTION_FUNCTION_HPP

//#include "third_party/boost/boost/phoenix/function/function_handling.hpp"
#include "third_party/boost/boost/phoenix/core/detail/function_eval.hpp"
#include "third_party/boost/boost/preprocessor/facilities/expand.hpp"
#include "third_party/boost/boost/preprocessor/logical/or.hpp"
#include "third_party/boost/boost/utility/result_of.hpp"

namespace boost { namespace phoenix
{
    /////////////////////////////////////////////////////////////////////////////
    // Functions
    /////////////////////////////////////////////////////////////////////////////

    // functor which returns our lazy function call extension
    template<typename F>
    struct function
    {
        function() {}

        function(F f)
          : f(f)
        {}

        template <typename Sig>
        struct result;

        typename detail::expression::function_eval<F>::type const
        operator()() const
        {
            return detail::expression::function_eval<F>::make(f);
        }

        // Bring in the rest
        #include "third_party/boost/boost/phoenix/function/detail/function_operator.hpp"

        F f;
    };
}

    template<typename F>
    struct result_of<phoenix::function<F>()>
      : phoenix::detail::expression::function_eval<F>
    {};

}

#endif

