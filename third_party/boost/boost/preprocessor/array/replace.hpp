# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_ARRAY_REPLACE_HPP
# define BOOST_PREPROCESSOR_ARRAY_REPLACE_HPP
#
# include "third_party/boost/boost/preprocessor/arithmetic/inc.hpp"
# include "third_party/boost/boost/preprocessor/array/elem.hpp"
# include "third_party/boost/boost/preprocessor/array/push_back.hpp"
# include "third_party/boost/boost/preprocessor/comparison/not_equal.hpp"
# include "third_party/boost/boost/preprocessor/config/config.hpp"
# include "third_party/boost/boost/preprocessor/control/deduce_d.hpp"
# include "third_party/boost/boost/preprocessor/control/iif.hpp"
# include "third_party/boost/boost/preprocessor/control/while.hpp"
# include "third_party/boost/boost/preprocessor/tuple/elem.hpp"
#
# /* BOOST_PP_ARRAY_REPLACE */
#
# define BOOST_PP_ARRAY_REPLACE(array, i, elem) BOOST_PP_ARRAY_REPLACE_I(BOOST_PP_DEDUCE_D(), array, i, elem)
# define BOOST_PP_ARRAY_REPLACE_I(d, array, i, elem) BOOST_PP_ARRAY_REPLACE_D(d, array, i, elem)
#
# /* BOOST_PP_ARRAY_REPLACE_D */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_ARRAY_REPLACE_D(d, array, i, elem) BOOST_PP_TUPLE_ELEM(5, 3, BOOST_PP_WHILE_ ## d(BOOST_PP_ARRAY_REPLACE_P, BOOST_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# else
#    define BOOST_PP_ARRAY_REPLACE_D(d, array, i, elem) BOOST_PP_ARRAY_REPLACE_D_I(d, array, i, elem)
#    define BOOST_PP_ARRAY_REPLACE_D_I(d, array, i, elem) BOOST_PP_TUPLE_ELEM(5, 3, BOOST_PP_WHILE_ ## d(BOOST_PP_ARRAY_REPLACE_P, BOOST_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# endif
#
# define BOOST_PP_ARRAY_REPLACE_P(d, state) BOOST_PP_NOT_EQUAL(BOOST_PP_TUPLE_ELEM(5, 0, state), BOOST_PP_ARRAY_SIZE(BOOST_PP_TUPLE_ELEM(5, 4, state)))
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_STRICT()
#    define BOOST_PP_ARRAY_REPLACE_O(d, state) BOOST_PP_ARRAY_REPLACE_O_I state
# else
#    define BOOST_PP_ARRAY_REPLACE_O(d, state) BOOST_PP_ARRAY_REPLACE_O_I(BOOST_PP_TUPLE_ELEM(5, 0, state), BOOST_PP_TUPLE_ELEM(5, 1, state), BOOST_PP_TUPLE_ELEM(5, 2, state), BOOST_PP_TUPLE_ELEM(5, 3, state), BOOST_PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define BOOST_PP_ARRAY_REPLACE_O_I(n, i, elem, res, arr) (BOOST_PP_INC(n), i, elem, BOOST_PP_ARRAY_PUSH_BACK(res, BOOST_PP_IIF(BOOST_PP_NOT_EQUAL(n, i), BOOST_PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# endif
