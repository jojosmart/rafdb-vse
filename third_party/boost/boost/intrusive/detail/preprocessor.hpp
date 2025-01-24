//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2008-2011. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_PREPROCESSOR_HPP
#define BOOST_INTRUSIVE_DETAIL_PREPROCESSOR_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include "third_party/boost/boost/intrusive/detail/config_begin.hpp"
#include "third_party/boost/boost/intrusive/detail/workaround.hpp"

#include "third_party/boost/boost/preprocessor/iteration/local.hpp" 
#include "third_party/boost/boost/preprocessor/punctuation/paren_if.hpp"
#include "third_party/boost/boost/preprocessor/punctuation/comma_if.hpp"
#include "third_party/boost/boost/preprocessor/control/expr_if.hpp"
#include "third_party/boost/boost/preprocessor/cat.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_trailing.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_shifted_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_shifted.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat.hpp"
#include "third_party/boost/boost/preprocessor/logical/not.hpp"
#include "third_party/boost/boost/preprocessor/arithmetic/sub.hpp"
#include "third_party/boost/boost/preprocessor/arithmetic/add.hpp"
#include "third_party/boost/boost/preprocessor/iteration/iterate.hpp"

#define BOOST_INTRUSIVE_MAX_CONSTRUCTOR_PARAMETERS 10

#define BOOST_INTRUSIVE_PP_IDENTITY(z, n, data) data

#define BOOST_INTRUSIVE_PP_DECLVAL(z, n, data) \
boost::move_detail::declval< BOOST_PP_CAT(P, n) >() \
//!

#define BOOST_INTRUSIVE_PP_TEMPLATE_PARAM_VOID_DEFAULT(z, n, data)   \
  BOOST_PP_CAT(class P, n) = void                                      \
//!

#include "third_party/boost/boost/intrusive/detail/config_end.hpp"

#endif //#ifndef BOOST_INTRUSIVE_DETAIL_PREPROCESSOR_HPP
