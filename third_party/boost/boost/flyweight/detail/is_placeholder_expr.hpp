/* Copyright 2006-2009 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_IS_PLACEHOLDER_EXPR_HPP
#define BOOST_FLYWEIGHT_DETAIL_IS_PLACEHOLDER_EXPR_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/mpl/apply.hpp"
#include "third_party/boost/boost/mpl/aux_/lambda_support.hpp"
#include "third_party/boost/boost/mpl/not.hpp"
#include "third_party/boost/boost/preprocessor/facilities/intercept.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"

namespace boost{

namespace flyweights{

namespace detail{

/* is_placeholder_expression<T> indicates whether T is an
 * MPL placeholder expression.
 */

template<typename T>
struct is_placeholder_expression_helper
{
  template<
    BOOST_PP_ENUM_PARAMS(
      BOOST_MPL_LIMIT_METAFUNCTION_ARITY,typename BOOST_PP_INTERCEPT)
  >
  struct apply{
    typedef int type;
  };

  BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_placeholder_expression_helper,(T))
};

template<typename T>
struct is_placeholder_expression:
  mpl::not_<is_same<
    typename mpl::apply<
      is_placeholder_expression_helper<T>,
      BOOST_PP_ENUM_PARAMS(
        BOOST_MPL_LIMIT_METAFUNCTION_ARITY,int BOOST_PP_INTERCEPT)
    >::type,
    int
  > >
{};

} /* namespace flyweights::detail */

} /* namespace flyweights */

} /* namespace boost */

#endif
