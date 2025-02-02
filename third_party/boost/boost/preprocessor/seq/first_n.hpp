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
# ifndef BOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
# define BOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
#
# include "third_party/boost/boost/preprocessor/config/config.hpp"
# include "third_party/boost/boost/preprocessor/control/if.hpp"
# include "third_party/boost/boost/preprocessor/seq/detail/split.hpp"
# include "third_party/boost/boost/preprocessor/tuple/eat.hpp"
# include "third_party/boost/boost/preprocessor/tuple/elem.hpp"
#
# /* BOOST_PP_SEQ_FIRST_N */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_SEQ_FIRST_N(n, seq) BOOST_PP_IF(n, BOOST_PP_TUPLE_ELEM, BOOST_PP_TUPLE_EAT_3)(2, 0, BOOST_PP_SEQ_SPLIT(n, seq (nil)))
# else
#    define BOOST_PP_SEQ_FIRST_N(n, seq) BOOST_PP_SEQ_FIRST_N_I(n, seq)
#    define BOOST_PP_SEQ_FIRST_N_I(n, seq) BOOST_PP_IF(n, BOOST_PP_TUPLE_ELEM, BOOST_PP_TUPLE_EAT_3)(2, 0, BOOST_PP_SEQ_SPLIT(n, seq (nil)))
# endif
#
# endif
