# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
# define BOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
#
# include "third_party/boost/boost/preprocessor/config/config.hpp"
# include "third_party/boost/boost/preprocessor/cat.hpp"
# include "third_party/boost/boost/preprocessor/tuple/elem.hpp"
# include "third_party/boost/boost/preprocessor/facilities/empty.hpp"
# include "third_party/boost/boost/preprocessor/detail/split.hpp"
#
# /* BOOST_PP_IS_EMPTY */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC() && ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I(x BOOST_PP_IS_EMPTY_HELPER)
#    define BOOST_PP_IS_EMPTY_I(contents) BOOST_PP_TUPLE_ELEM(2, 1, (BOOST_PP_IS_EMPTY_DEF_ ## contents()))
#    define BOOST_PP_IS_EMPTY_DEF_BOOST_PP_IS_EMPTY_HELPER 1, 1 BOOST_PP_EMPTY
#    define BOOST_PP_IS_EMPTY_HELPER() , 0
# else
#    if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#        define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I(BOOST_PP_IS_EMPTY_HELPER x ())
#        define BOOST_PP_IS_EMPTY_I(test) BOOST_PP_IS_EMPTY_II(BOOST_PP_SPLIT(0, BOOST_PP_CAT(BOOST_PP_IS_EMPTY_DEF_, test)))
#        define BOOST_PP_IS_EMPTY_II(id) id
#    else
#        define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I((BOOST_PP_IS_EMPTY_HELPER x ()))
#        define BOOST_PP_IS_EMPTY_I(par) BOOST_PP_IS_EMPTY_II ## par
#        define BOOST_PP_IS_EMPTY_II(test) BOOST_PP_SPLIT(0, BOOST_PP_CAT(BOOST_PP_IS_EMPTY_DEF_, test))
#    endif
#    define BOOST_PP_IS_EMPTY_HELPER() 1
#    define BOOST_PP_IS_EMPTY_DEF_1 1, BOOST_PP_NIL
#    define BOOST_PP_IS_EMPTY_DEF_BOOST_PP_IS_EMPTY_HELPER 0, BOOST_PP_NIL
# endif
#
# endif
