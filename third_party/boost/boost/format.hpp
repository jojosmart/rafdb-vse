// ----------------------------------------------------------------------------
// format.hpp :  primary header
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page


// ----------------------------------------------------------------------------

#ifndef BOOST_FORMAT_HPP
#define BOOST_FORMAT_HPP

#include <vector>
#include <string>
#include "third_party/boost/boost/detail/workaround.hpp"
#include "third_party/boost/boost/config.hpp"

#ifndef BOOST_NO_STD_LOCALE
#include <locale>
#endif

// ***   Compatibility framework
#include "third_party/boost/boost/format/detail/compat_workarounds.hpp"

#ifdef BOOST_NO_LOCALE_ISIDIGIT
#include <cctype>  // we'll use the non-locale  <cctype>'s std::isdigit(int)
#endif

// ****  Forward declarations ----------------------------------
#include "third_party/boost/boost/format/format_fwd.hpp"     // basic_format<Ch,Tr>, and other frontends
#include "third_party/boost/boost/format/internals_fwd.hpp"  // misc forward declarations for internal use

// ****  Auxiliary structs (stream_format_state<Ch,Tr> , and format_item<Ch,Tr> )
#include "third_party/boost/boost/format/internals.hpp"    

// ****  Format  class  interface --------------------------------
#include "third_party/boost/boost/format/format_class.hpp"

// **** Exceptions -----------------------------------------------
#include "third_party/boost/boost/format/exceptions.hpp"

// **** Implementation -------------------------------------------
#include "third_party/boost/boost/format/format_implementation.hpp"   // member functions
#include "third_party/boost/boost/format/group.hpp"                   // class for grouping arguments
#include "third_party/boost/boost/format/feed_args.hpp"               // argument-feeding functions
#include "third_party/boost/boost/format/parsing.hpp"                 // format-string parsing (member-)functions

// **** Implementation of the free functions ----------------------
#include "third_party/boost/boost/format/free_funcs.hpp"


// *** Undefine 'local' macros :
#include "third_party/boost/boost/format/detail/unset_macros.hpp"

#endif // BOOST_FORMAT_HPP
