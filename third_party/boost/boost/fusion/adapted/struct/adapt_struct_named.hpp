/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2009-2010 Hartmut Kaiser
    Copyright (c) 2010-2011 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_NAMED_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_NAMED_HPP

#include "third_party/boost/boost/fusion/adapted/struct/adapt_struct.hpp"
#include "third_party/boost/boost/fusion/adapted/struct/detail/proxy_type.hpp"
#include "third_party/boost/boost/preprocessor/empty.hpp"

#define BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_0(X, Y)                          \
    (X, obj.Y) BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_1
#define BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_1(X, Y)                          \
    (X, obj.Y) BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_0
#define BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_0_END
#define BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_1_END

#define BOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                     \
    WRAPPED_TYPE, NAMESPACE_SEQ, NAME, ATTRIBUTES)                              \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_DEFINE_PROXY_TYPE_IMPL(                           \
        WRAPPED_TYPE,(0)NAMESPACE_SEQ,NAME)                                     \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_AS_VIEW(                                          \
        BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ)NAME,  \
        BOOST_PP_CAT(                                                           \
            BOOST_FUSION_ADAPT_STRUCT_NAMED_FILLER_0 ATTRIBUTES,_END))

#define BOOST_FUSION_ADAPT_STRUCT_NAMED(WRAPPED_TYPE, NAME, ATTRIBUTES)         \
    BOOST_FUSION_ADAPT_STRUCT_NAMED_NS(                                         \
        WRAPPED_TYPE,(boost)(fusion)(adapted),NAME,ATTRIBUTES)

#endif
