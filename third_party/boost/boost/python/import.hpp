// Copyright Stefan Seefeld 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef IMPORT_SS20050624_HPP
# define IMPORT_SS20050624_HPP

# include "third_party/boost/boost/python/object.hpp"
# include "third_party/boost/boost/python/str.hpp"

namespace boost 
{ 
namespace python 
{

// Import the named module and return a reference to it.
object BOOST_PYTHON_DECL import(str name);

}
}

#endif
