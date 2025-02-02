# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
# define BOOST_PREPROCESSOR_ARRAY_TO_SEQ_HPP
#
# include "third_party/boost/boost/preprocessor/cat.hpp"
# include "third_party/boost/boost/preprocessor/config/config.hpp"
# include "third_party/boost/boost/preprocessor/tuple/to_seq.hpp"
#
# /* BOOST_PP_ARRAY_TO_SEQ */
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#    define BOOST_PP_ARRAY_TO_SEQ(array) BOOST_PP_ARRAY_TO_SEQ_I(BOOST_PP_TUPLE_TO_SEQ, array)
#    define BOOST_PP_ARRAY_TO_SEQ_I(m, args) BOOST_PP_ARRAY_TO_SEQ_II(m, args)
#    define BOOST_PP_ARRAY_TO_SEQ_II(m, args) BOOST_PP_CAT(m ## args,)
# elif BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_ARRAY_TO_SEQ(array) BOOST_PP_ARRAY_TO_SEQ_I(array)
#    define BOOST_PP_ARRAY_TO_SEQ_I(array) BOOST_PP_TUPLE_TO_SEQ ## array
# else
#    define BOOST_PP_ARRAY_TO_SEQ(array) BOOST_PP_TUPLE_TO_SEQ array
# endif
#
# endif
