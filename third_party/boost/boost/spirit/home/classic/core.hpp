/*=============================================================================
    Copyright (c) 1998-2003 Joel de Guzman
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2001-2003 Hartmut Kaiser
    Copyright (c) 2002-2003 Martin Wille
    Copyright (c) 2002 Raghavendra Satish
    Copyright (c) 2001 Bruce Florman
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_CORE_MAIN_HPP)
#define BOOST_SPIRIT_CORE_MAIN_HPP

#include "third_party/boost/boost/spirit/home/classic/version.hpp"
#include "third_party/boost/boost/spirit/home/classic/debug.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Core includes
//
///////////////////////////////////////////////////////////////////////////////

//  Spirit.Core.Kernel
#include "third_party/boost/boost/spirit/home/classic/core/config.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/nil.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/match.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/parser.hpp"

//  Spirit.Core.Primitives
#include "third_party/boost/boost/spirit/home/classic/core/primitives/primitives.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/primitives/numerics.hpp"

//  Spirit.Core.Scanner
#include "third_party/boost/boost/spirit/home/classic/core/scanner/scanner.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/scanner/skipper.hpp"

//  Spirit.Core.NonTerminal
#include "third_party/boost/boost/spirit/home/classic/core/non_terminal/subrule.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/non_terminal/rule.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/non_terminal/grammar.hpp"

//  Spirit.Core.Composite
#include "third_party/boost/boost/spirit/home/classic/core/composite/actions.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/composite.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/directives.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/epsilon.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/sequence.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/sequential_and.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/sequential_or.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/alternative.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/difference.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/intersection.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/exclusive_or.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/kleene_star.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/positive.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/optional.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/list.hpp"
#include "third_party/boost/boost/spirit/home/classic/core/composite/no_actions.hpp"

//  Deprecated interface includes
#include "third_party/boost/boost/spirit/home/classic/actor/assign_actor.hpp"
#include "third_party/boost/boost/spirit/home/classic/actor/push_back_actor.hpp"

#if defined(BOOST_SPIRIT_DEBUG)
    //////////////////////////////////
    #include "third_party/boost/boost/spirit/home/classic/debug/parser_names.hpp"

#endif // BOOST_SPIRIT_DEBUG

#endif // BOOST_SPIRIT_CORE_MAIN_HPP

