// Copyright Gottfried Gan�auge 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
/*
 * Generic Return value converter generator for opaque C++-pointers
 */
# ifndef RETURN_OPAQUE_POINTER_HPP_
# define RETURN_OPAQUE_POINTER_HPP_

# include "third_party/boost/boost/python/detail/prefix.hpp"
# include "third_party/boost/boost/python/opaque_pointer_converter.hpp"
# include "third_party/boost/boost/python/detail/force_instantiate.hpp"
# include "third_party/boost/boost/python/to_python_value.hpp"
# include "third_party/boost/boost/python/detail/value_arg.hpp"
# include "third_party/boost/boost/mpl/assert.hpp"

namespace boost { namespace python {

namespace detail
{
  template <class Pointee>
  static void opaque_pointee(Pointee const volatile*)
  {
      force_instantiate(opaque<Pointee>::instance);
  }
}

struct return_opaque_pointer
{
    template <class R>
    struct apply
    {
        BOOST_MPL_ASSERT_MSG( is_pointer<R>::value, RETURN_OPAQUE_POINTER_EXPECTS_A_POINTER_TYPE, (R));
        
        struct type :  
          boost::python::to_python_value<
              typename detail::value_arg<R>::type
          >
        {
            type() { detail::opaque_pointee(R()); }
        };
    };
};

}} // namespace boost::python
# endif // RETURN_OPAQUE_POINTER_HPP_
