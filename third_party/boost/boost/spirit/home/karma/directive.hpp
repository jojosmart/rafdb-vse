//  Copyright (c) 2001-2011 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_DIRECTIVE_FEB_21_2007_0833PM)
#define BOOST_SPIRIT_KARMA_DIRECTIVE_FEB_21_2007_0833PM

#if defined(_MSC_VER)
#pragma once
#endif

#include "third_party/boost/boost/spirit/home/karma/directive/encoding.hpp"

///////////////////////////////////////////////////////////////////////////////
//  directives related to alignment 
//  left_align[...], right_align[...], center[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/left_alignment.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/right_alignment.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/center_alignment.hpp"

///////////////////////////////////////////////////////////////////////////////
//  directives related to truncating length
//  maxwidth[...], columns[]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/maxwidth.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/columns.hpp"

///////////////////////////////////////////////////////////////////////////////
//  directives related to character case
//  lower[...] and upper[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/upper_lower_case.hpp"

///////////////////////////////////////////////////////////////////////////////
//  directives related to delimiting generators 
//  delimit[...] and verbatim[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/verbatim.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/delimit.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/no_delimit.hpp"

///////////////////////////////////////////////////////////////////////////////
//  repeat directives
//  repeat[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/repeat.hpp"

///////////////////////////////////////////////////////////////////////////////
//  omit, skip, and duplicate directives
//  omit[...], skip[...], duplicate[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/omit.hpp"
#include "third_party/boost/boost/spirit/home/karma/directive/duplicate.hpp"

///////////////////////////////////////////////////////////////////////////////
//  buffer directive
//  buffer[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/buffer.hpp"

///////////////////////////////////////////////////////////////////////////////
//  strict and relaxed directives
//  strict[...], relaxed[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/strict_relaxed.hpp"

///////////////////////////////////////////////////////////////////////////////
//  as_string and as_wstring directives
//  as_string[...], as_wstring[...]
///////////////////////////////////////////////////////////////////////////////
#include "third_party/boost/boost/spirit/home/karma/directive/as.hpp"

#endif
