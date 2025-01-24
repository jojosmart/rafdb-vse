/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_VECTOR20_05052005_0205)
#define FUSION_VECTOR20_05052005_0205

#include "third_party/boost/boost/fusion/container/vector/vector20_fwd.hpp"
#include "third_party/boost/boost/fusion/support/sequence_base.hpp"
#include "third_party/boost/boost/fusion/support/detail/access.hpp"
#include "third_party/boost/boost/fusion/iterator/next.hpp"
#include "third_party/boost/boost/fusion/iterator/deref.hpp"
#include "third_party/boost/boost/fusion/sequence/intrinsic/begin.hpp"
#include "third_party/boost/boost/fusion/container/vector/detail/at_impl.hpp"
#include "third_party/boost/boost/fusion/container/vector/detail/value_at_impl.hpp"
#include "third_party/boost/boost/fusion/container/vector/detail/begin_impl.hpp"
#include "third_party/boost/boost/fusion/container/vector/detail/end_impl.hpp"

#include "third_party/boost/boost/mpl/void.hpp"
#include "third_party/boost/boost/mpl/int.hpp"
#include "third_party/boost/boost/mpl/at.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"
#include "third_party/boost/boost/mpl/vector/vector20.hpp"
#include "third_party/boost/boost/type_traits/is_convertible.hpp"
#include "third_party/boost/boost/utility/enable_if.hpp"

#include "third_party/boost/boost/preprocessor/dec.hpp"
#include "third_party/boost/boost/preprocessor/iteration/iterate.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_shifted.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "third_party/boost/boost/preprocessor/repetition/repeat_from_to.hpp"

#if !defined(BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include "third_party/boost/boost/fusion/container/vector/detail/preprocessed/vector20.hpp"
#else
#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/vector20.hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace boost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

// expand vector11 to vector20
#define BOOST_PP_FILENAME_1 "third_party/boost/boost/fusion/container/vector/detail/vector_n.hpp"
#define BOOST_PP_ITERATION_LIMITS (11, 20)
#include BOOST_PP_ITERATE()

}}

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

