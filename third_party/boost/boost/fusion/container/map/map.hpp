/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_MAP_07212005_1106)
#define FUSION_MAP_07212005_1106

#include "third_party/boost/boost/fusion/support/pair.hpp"
#include "third_party/boost/boost/fusion/support/category_of.hpp"
#include "third_party/boost/boost/fusion/support/detail/access.hpp"
#include "third_party/boost/boost/fusion/container/map/map_fwd.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/at_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/value_at_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/begin_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/end_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/value_of_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/deref_data_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/deref_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/key_of_impl.hpp"
#include "third_party/boost/boost/fusion/container/map/detail/value_of_data_impl.hpp"
#include "third_party/boost/boost/fusion/container/vector/vector.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"
#include "third_party/boost/boost/mpl/bool.hpp"

#if !defined(BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include "third_party/boost/boost/fusion/container/map/detail/preprocessed/map.hpp"
#else
#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/map" FUSION_MAX_MAP_SIZE_STR ".hpp")
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
    struct void_;
    struct fusion_sequence_tag;

    template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, typename T)>
    struct map : sequence_base<map<BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, T)> >
    {
        struct category : random_access_traversal_tag, associative_tag {};

        typedef map_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;

        typedef vector<
            BOOST_PP_ENUM_PARAMS(FUSION_MAX_MAP_SIZE, T)>
        storage_type;

        typedef typename storage_type::size size;

        map()
            : data() {}

        template <typename Sequence>
        map(Sequence const& rhs)
            : data(rhs) {}

        #include "third_party/boost/boost/fusion/container/map/detail/map_forward_ctor.hpp"

        template <typename T>
        map&
        operator=(T const& rhs)
        {
            data = rhs;
            return *this;
        }

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:

        storage_type data;
    };
}}

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
